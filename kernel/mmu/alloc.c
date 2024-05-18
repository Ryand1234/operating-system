#include<mmu/vmm.h>
#include<stdio.h>
#include<stdint.h>
void kfree(void  *v_addr)
{
//	printf("KFREEEEEEEE\n");
	if(v_addr == (void*)0)
		return ;

	struct kmalloc_header *chunk, *other;

	chunk = (struct kmalloc_header*) ((uint32_t)v_addr - sizeof(struct kmalloc_header));

	//printf("FREE CHUNK: %d, %d, %d\n", chunk, chunk->size, chunk->used);

	chunk->used = 0;

	kmalloc_used -= chunk->size;
	while((other = (struct kmalloc_header*)((char*)chunk + chunk->size))  && other < (struct kmalloc_header*) kern_heap && other->used == 0)
	{
		chunk->size = chunk->size + other->size;
	}

}
void *ksbrk(uint16_t n)
{
	struct kmalloc_header* chunk;
	char *p_addr;
	uint32_t i;

	if((kern_heap + (n*PAGESIZE)) > (char*) KERN_HEAP_LIM) {
		printf("PANIC ksbrk(): no memory left\n");
		return (char *) -1;
	}

	chunk = (struct kmalloc_header *) kern_heap;

	for(i = 0; i < n; i++) {
		p_addr = get_page_frame();
		printf("P ADDR: %d\n", p_addr);
		if((uint32_t)(p_addr) < 0) {
			printf("PANIC ksbrk(): no page left\n");
			return (char*) -1;
		}
		pd0_add_page(kern_heap, p_addr, 0);
		kern_heap += PAGESIZE;
	}

	chunk->size = PAGESIZE*n;
	chunk->used = 0;
	return chunk;
}
	void *kmalloc(uint32_t size)
	{
		if(size <= 0)
		{
			return 0;
		}


		uint32_t realsize = sizeof(struct kmalloc_header) + size;
		if(realsize < KMALLOC_MINSIZE)
		{
			realsize = KMALLOC_MINSIZE;
		}

		printf("Size required: %d\n", realsize);
		struct kmalloc_header *chunk, *other;
		chunk = (struct kmalloc_header*) &end;
		while(chunk->used || chunk->size < realsize)
		{
			if(chunk->size == 0) {
				printf("PANIC: kmalloc(): corrupted chunk on %d with null size (heap %d). System halt", chunk, kern_heap);
				asm("hlt");
				return 0;
			}
			chunk = (struct kmalloc_header*) ((char*)chunk + chunk->size);
		//	printf("CHUNK:%d,  %d, %d, HEAP: %d\n", chunk->size, chunk->used, chunk, (struct kmalloc_header*)kern_heap);
			if(chunk == (struct kmalloc_header*) kern_heap || ((struct kmalloc_header*) kern_heap - chunk) < realsize)
			{
				if((int)(ksbrk((realsize/PAGESIZE)+1)) < 0)
				{
					printf("No memory left %d, heap: %d", chunk, kern_heap);
					asm("hlt");
					return 0;
				}
			} else if(chunk > (struct kmalloc_header*) kern_heap)
			{
				printf("Got address %d while heap limit is %d", chunk, kern_heap);
				asm("hlt");
				return 0;
			}
		}

		if(chunk->size - realsize < KMALLOC_MINSIZE)
		{
			chunk->used = 1;
		} else {
			other = (struct kmalloc_header*)((char*) chunk+ realsize);
			other->size = (uint32_t) (chunk-size - realsize);
			other->used = 0;
			chunk->size = realsize;
			chunk->used = 1;
		}

		kmalloc_used += realsize;

		return (char *) chunk + sizeof(struct kmalloc_header);
	}
	

