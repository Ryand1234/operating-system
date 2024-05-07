#ifdef ALLOC_H
#define ALLOC_H

extern "C" {
	int kbsrk(uint32_t)
	void *kmalloc(uint32_t);
	void kfree(void *);
}

#endif
