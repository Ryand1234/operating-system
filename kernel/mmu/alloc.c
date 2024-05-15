#include<mmu/vmm.h>
#include<stdio.h>
#include<stdint.h>
void kfree(void  *v_addr)
{
	printf("KFREEEEEEEE\n");

}
void *ksbrk(uint32_t n)
{
	//printf("BREakkkkkk\n");

	struct kmalloc_header* chunk = (struct kmalloc_header *) kern_heap;
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

		struct kmalloc_header *chunk = (struct kmalloc_header*) KERN_HEAP;
		while(chunk->used || chunk->size < realsize)
		{
			//printf("chunk: %d, %d\n", chunk->used, chunk->size);
			chunk = (struct kmalloc_header*) ((char*)chunk + chunk->size);
			printf("C: %d, S: %d, U: %d, h: %d", chunk, chunk->used, chunk->size, kern_heap);
			if(chunk == (struct kmalloc_header*) KERN_HEAP)
			{
				if((int)ksbrk(realsize/PAGESIZE) < 0)
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
			struct kmalloc_header* other = (struct kmalloc_header*)((char*) chunk+ realsize);
			other->size = (uint32_t) (chunk-size - realsize);
			other->used = 0;
			chunk->size = realsize;
			chunk->used = 1;
		}

		kmalloc_used += realsize;

		return (char *) chunk + sizeof(struct kmalloc_header);
	}
	

