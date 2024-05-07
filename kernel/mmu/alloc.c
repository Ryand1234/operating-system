#include<mmu.h>
#include<stdio.h>

extern "C" {
	void *kmalloc(uint32_t size)
	{
		if(size <= 0)
		{
			return 0;
		}


		uint32_t realsize = sizeof(struct kmalloc_header) + size;
		if(realsize < KMALLOC_MINSIZE)
		{
			realsize = KMALLOCSIZE;
		}

		chunk = (struct *kmalloc_header) KERN_HEAP;
		while(chunk->used || chunk->size < realsize)
		{
			printf("chunk: %d, %d\n", chunk->used, chunk->size);
			chunk = (struct *KMALLOC_HEADER) ((char*)chunk + chunk->size);
			if(chunk == (struct *KMALLOC_HEADER) kern_heap)
			{
				if((int)ksbrk(realsize/PAGESIZE) < 0)
				{
					printf("No memory left %d, heap: %d", chunk, kern_heap);
					asm("hlt");
					return 0;
				}
			} else if(chunk > (struct *KMALLOC_HEADER) kern_heap)
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
			other = (struct *kmalloc_header)((chunk*) chunk + realsize)
			other->size = chunk-size - realsize;
			other->used = 0;
			chunk->size = realsize;
			chunk->used = 1;
		}

		kmalloc_used += realsize;

		return (char *) chunk + sizeof(struct kmalloc_header);
	}
}
