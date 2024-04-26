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
	uint16_t type;
	uint16_t offset16_31;
} __attribute__ ((packed));


#define GDTSIZE 0xFF
#define IDTSIZE 1
#define STACKSIZE 4096


#define GDT_BASE 0x00000
#define IDT_BASE 0x00000


#define INTGATE 0x8E
#define TRAPGATE 0xEF

#define DESC_NULL 0x0000
#define DESC_KERNEL_CODE 0x0008
#define DESC_KERNEL_DATA 0x0010
#define DESC_USER_CODE 0x0018
#define DESC_USER_DATA 0x0020


void create_gdt_descriptor(uint32_t base, uint32_t limit, uint8_t access, uint8_t flags, struct gdtdesc *);

void init_gdt(void);

void init_idt(void);

void create_idt_descriptor(uint16_t, uint32_t, uint16_t, struct idtdesc *);

void install_irq(unsigned int num, unsigned int irq);

void init_pic(void);

void setup_isr(void);

#endif
