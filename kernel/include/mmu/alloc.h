#ifndef MMU_ALLOC_H
#define MMU_ALLOC_H

#include<stdint.h>
void *ksbrk(uint32_t);
void *kmalloc(uint32_t);
void kfree(void *);


#endif
