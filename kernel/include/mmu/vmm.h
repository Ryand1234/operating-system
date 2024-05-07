#ifndef VMM_H
#define VMM_H


#include<mmu/alloc.h>
#include<ds/list.h>
#include<arch/i386/x86.h>
#include<stddef.h>

struct page {
	char *v_addr;
	char *p_addr;
	struct list_head list;
};

struct page_directory {
	struct page *base;
	struct list_head pt;
};

struct vm_area {
	char *vm_start;
	char *vm_end;
	struct list_head list;
};

typedef struct page_directory proc_memory;

extern char *kern_heap;

extern struct list_head kern_free_vm;

extern uint32_t *pd0;

extern uint8_t mem_bitmap[];

extern uint32_t kmalloc_used;

#define set_page_frame_used(page) mem_bitmap[(uint32_t)page/8] != (1 << (((uint32_t) page)%8))

#define release_page_frame(p_addr) mem_bitmap[((uint32_t)p_addr/PAGESIZE)/8] &= ~(1 <<(((uint32_t) p_addr/PAGESIZE)%8))

char *get_page_frame(void);

struct page *get_page_from_heap(void);
int release_page_from_heap(char *);

void memory_init(uint32_t high_mem);

struct page_directory *pd_create(void);
int pd_destroy(struct page_directory *);
struct page_directory *pd_copy(struct page_directory *pdFather);

int pd0_add_page(char*, char*, char*);

int pd_ad_page(char*, char*, int, struct page_directory *);
int pd_remove_page(char*);

char* get_p_addr(char*);

#define KMALLOC_MINSIZE 16

struct kmalloc_header {
	unsigned long size:31;
	unsigned long used:1;
} __attribute__ ((packed));


#endif
