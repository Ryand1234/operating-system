#include<mmu/vmm.h>
	char *kern_heap;
	struct list_head kern_free_vm;
	uint32_t *pd0 = (uint32_t *) KERN_PDIR;
	char *pg0 = (char*) 0;
	char *pg1 = (char*) KERN_PG_1;
	char *pg1_end = (char*) KERN_PG_1_LIM;
	uint8_t mem_bitmap[RAM_MAXPAGE /8];

	uint32_t kmalloc_used = 0;

	char* get_page_frame(void)
	{
		int byte, bit;
		int page = -1;

		for(byte = 0; byte < RAM_MAXSIZE / 8; byte++)
		{
			if(mem_bitmap[byte] != 0xFF)
			{
				for(bit = 0; bit < 8; bit++)
				{
					if(!(mem_bitmap[byte] & (1 << bit))) {
						page = 8 * byte + bit;
						set_page_frame_used(page);
						return (char*) (page * PAGESIZE);
					}
				}
			}
		}
		return (char*) -1;
	}

	struct page* get_page_from_heap(void)
	{
		struct page *pg;
		struct vm_area *area;
		char *v_addr, *p_addr;

		p_addr = get_page_frame();
		if((int)(p_addr) < 0) {
			printf("PANIC: get_page_from_heap(); no page frame available. Can't work!\n");
		}

		if(is_list_empty(&kern_free_vm)) {
			printf("PANIC: get_page_from_heap(): no memory left in page heap. System can't work\n");
		}

		area = list_first_entry(&kern_free_vm, struct vm_area, list);
		v_addr = area->vm_start;

		area->vm_start += PAGESIZE;

		if(area->vm_start == area->vm_end) {
			list_del(&area->list);
			kfree(area);
		}

		pd0_add_page(v_addr, p_addr, 0);

		pg = (struct page*) kmalloc(sizeof(struct page));
		pg->v_addr = v_addr;
		pg->p_addr = p_addr;
		pg->list.next = 0;
		pg->list.prev = 0;

		return pg;
	}

/*	int release_page_from_heap(char *v_addr) 
	{
		struct vm_area *next_area, *prev_area, *new_area;
		char *p_addr;

		p_addr = get_p_addr(v_addr);
		if(p_addr) {
			release_page_frame(p_addr);
		} else {
			printf("WARNING: release_page_from_heap(): no page frame associated with v_addr: %d\n", v_addr);
			return 1;
		}

		pd_remove_page(v_addr);

		list_for_each_entry(next_area, &kern_free_vm, list) {
			if(next_area->vm_start > v_addr){
				break;
			}
		}

		prev_area = list_entry(next_area->list.prev, struct vm_area, list);

		if(prev_area->vm_end == v_addr) {
			prev_area->vm_end += PAGESIZE;
			if(prev_area->vm_end == next_area->vm_start) {
				prev_area = next_area->vm_end;
				list_del(&next_area->list);
				kfree(next_area);
			}
		} else if(next_area->vm_start == v_addr + PAGESIZE) {
			next_area->vm_start = v_addr;
		} else if (next_area->vm_start > v_addr + PAGESIZE) {
			new_area = (struct vm_area*) kmalloc(sizeof(struct vm_area));
			new_area->vm_start = v_addr;
			new_area->vm_end = v_addr + PAGESIZE;
			list_add(&new_area->list, &prev_area->list);
		} else {
			printf("\nPANIC: release_page_from_heap(): COrrupted list");
			asm("hlt");
		}

		return 0;
	}
*/
	void memory_init(uint32_t high_mem)
	{
		int pg, pg_limit;
		uint32_t i;
		struct vm_area *p, *pd;

		pg_limit = (high_mem * 1024) / PAGESIZE;

		for(pg = 0; pg < pg_limit/8; pg++){
			mem_bitmap[pg] = 0;
		}
		
		for(pg = pg_limit/8; pg < RAM_MAXPAGE /8; pg++){
			mem_bitmap[pg] = 0xFF;
		}

		/*for(pg = PAGE(0x0); pg < (uint32_t)(PAGE((uint32_t) pg1_end)); pg++)
		{
			set_page_frame_used(pg);
		}

		pd[0] = ((uint32_t) pg0 | (PG_PRESENT | PG_WRITE | PG_4MB));
		pd[1] = ((uint32_t) pg1 | (PG_PRESENT | PG_WRITE | PG_4MB));

		for(i = 2; i < 1023; i++)
		{
			pd0[i] = ((uint32_t) pg1 + PAGESIZE*i (PG_PRESENT | PG_WRITE));
		}

		pd0[1023] = ((uint32_t) pd0 | (PG_PRESENT | PG_WRITE));*/

		asm volatile(" mov %0, %%eax \n \
			mov %%eax, %%cr3 \n \
			mov %%cr4, %%eax \n \
			or %2, %%eax \n \
			mov %%eax, %%cr4 \n \
			mov %%cr0, %%eax \n \
			or %1, %%eax \n \
			mov %%eax, %%cr0":: "m"(pd0), "i"(PAGING_FLAG), "i"(PSE_FLAG));


		kern_heap = (char*) KERN_HEAP;
		ksbrk(1);

		p = (struct vm_area*) kmalloc(sizeof(struct vm_area));
		p->vm_start = (char*) KERN_PG_HEAP;
		p->vm_end = (char*) KERN_PG_HEAP_LIM;
		INIT_LIST_HEAD(&kern_free_vm);
		list_add(&p->list, &kern_free_vm);

		return;
	}

	/*struct page_directory *pd_create(void)
	{
		struct page_directory *pd;
		uint32_t *pdir;
		uint32_t i;

		pd = (struct page_directory *) kmalloc(sizeof(struct page_directory));
		pd->base = get_page_from_heap();

		pdir = (uint32_t) pd->base->v_addr;
		for(i = 0; i < 256; i++)
			pdir[i] = pd0[i];

		pdir[1023] = ((uint32_t) pd->base->p_addr | (PG_PRESENT | PG_WRITE));

		INIT_LIST_HEAD(&pd->pt);
		return pd;
	}

	struct page_directory *pd_copy(struct page_directory * pdFather)
	{
		struct page_directory *pd;
		uint32_t *pdir;
		int ;

		pd = (struct page_directory *) kmalloc(sizeof(struct page_directory));
		pd->base = get_page_from_heap();

		pdir = (uint32_t *) pd->base->v_addr;

		
	}*/

