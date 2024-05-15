#ifndef ARCH_I386_X86_H
#define ARCH_I386_X86_H

#include<stddef.h>
#include<stdint.h>

struct gdtr {
	uint16_t limit;
	uint32_t base;
} __attribute__ ((packed));

struct gdtdesc {
	uint16_t limit0_15;
	uint16_t base0_15;
	uint8_t base16_23;
	uint8_t access_bytes;
	uint8_t flags:4;
	uint8_t limit16_19;
} __attribute__ ((packed));

struct idtr {
	uint16_t limit;
	uint32_t base;
} __attribute ((packed));

struct idtdesc {
	uint16_t offset0_15;
	uint16_t select;
	uint8_t alwaysZero;
	uint8_t access_gran;
	uint16_t offset16_31;
} __attribute__ ((packed));


#define GDTSIZE 0xFF
#define IDTSIZE 0xFF
#define STACKSIZE 4096


#define GDT_BASE 0x00000
#define IDT_BASE 0x00000


#define INTGATE 0x8E
#define TRAPGATE 0xEF



#define	KERN_PDIR			0x00001000
#define	KERN_STACK			0x0009FFF0
#define	KERN_BASE			0x00100000
#define KERN_PG_HEAP		0x00800000
#define KERN_PG_HEAP_LIM	0x10000000
#define KERN_HEAP			0x20000000
#define KERN_HEAP_LIM		0x70000000


#define KERN_PG_1			0x400000
#define KERN_PG_1_LIM 		0x800000

#define	VADDR_PD_OFFSET(addr)	((addr) & 0xFFC00000) >> 22
#define	VADDR_PT_OFFSET(addr)	((addr) & 0x003FF000) >> 12
#define	VADDR_PG_OFFSET(addr)	(addr) & 0x00000FFF
#define PAGE(addr)		(addr) >> 12

#define	PAGING_FLAG 		0x80000000	/* CR0 - bit 31 */
#define PSE_FLAG			0x00000010	/* CR4 - bit 4  */

#define PG_PRESENT			0x00000001	/* page directory / table */
#define PG_WRITE			0x00000002
#define PG_USER				0x00000004
#define PG_4MB				0x00000080

#define	PAGESIZE 			4096
#define	RAM_MAXSIZE			0x100000000
#define	RAM_MAXPAGE			0x100000

void create_gdt_descriptor(uint32_t base, uint32_t limit, uint8_t access, uint8_t flags, struct gdtdesc *);

void init_gdt(void);

void init_idt(void);

void create_idt_descriptor(uint16_t, uint32_t, uint8_t, struct idtdesc *);

void install_irq(unsigned int num, unsigned int irq);

void init_pic(void);

void setup_isr(void);

#endif
