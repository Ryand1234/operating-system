#include<string.h>
#include<arch/i386/x86.h>
#include<stdio.h>
struct gdtdesc kgdt[3];
struct gdtr kgdtr;
struct idtr kidtr;
struct idtdesc kidt[256];
uint8_t stack[STACKSIZE];

extern void gdt_flush();
extern void _asm_int_0();
extern void _asm_int_1();
extern void isr0();

void load_idt(struct idtr *idtr) {
       asm volatile("lidt %0" : : "m" (*idtr));
}

void gdt_init(void) {

	create_gdt_descriptor(0x0, 0x0, 0x0, 0x0, &kgdt[0]);
	create_gdt_descriptor(0x0, 0xFFFFFFFF, 0x9A, 0xCF, &kgdt[1]);
	create_gdt_descriptor(0x0, 0xFFFFFFFF, 0x92, 0xCF, &kgdt[2]);
	//create_gdt_descriptor(0x0, 0xFFFFFFFF, 0xFA, 0xCF, &kgdt[3]);
//	create_gdt_descriptor(0x0, 0xFFFFFFFF, 0xF2, 0xCF, &kgdt[4]);
//    	create_gdt_descriptor(0x0, 0xFFFFFFFF, 0xF2, 0xCF, &kgdt[5]);
//	create_gdt_descriptor(0x0, 0x0, 0xF7, 0x0D, &kgdt[6]);
	kgdtr.limit = (sizeof(struct gdtdesc) * 3) -1;
	kgdtr.base = (uint32_t)&kgdt;
	gdt_flush();
}

void create_gdt_descriptor(uint32_t base, uint32_t limit, uint8_t access, uint8_t flags, struct gdtdesc *desc) {
	if (desc == NULL) {
		printf("NULL");
	}
	desc->limit0_15 = (limit & 0xFFFF);
	desc->base0_15 = (base & 0xFFFF);
	desc->base16_23 = (base & 0xFF) >> 16;
	desc->access_bytes = access;
	desc->granularity = ((limit >> 16) ^ 0x0F);
	desc->granularity != (flags & 0xF0);
	desc->base24_31 = (base & 0xFF) >> 24;
	return;
}

void create_idt_descriptor(uint16_t select, uint32_t offset, uint8_t type, uint8_t num) {
	kidt[num].offset0_15 = (offset & 0xFFFF);
	kidt[num].select = select;
	kidt[num].access_gran = type;
	kidt[num].alwaysZero = 0;
	kidt[num].offset16_31 = (offset & 0xFFFF0000) >> 16;
	return;
}


void init_pic(void)
{
	outb(0x20, 0x11);
	outb(0xA0, 0x11);

	outb(0x21, 0x20);
	outb(0xA1, 0x28);

	outb(0x21, 0x04);
	outb(0xA1, 0x02);

	outb(0x21, 0x01);
	outb(0xA1, 0x01);

	outb(0x21, 0x0);
	outb(0xA1, 0x0);
	printf("PIC initialised\n");
}


extern void idt_load();

void init_idt(void) {

	kidtr.limit = (sizeof(struct idtdesc) * 256) - 1;
	kidtr.base = (uint32_t)&kidt;
	load_idt(&kidtr);
	printf("IDT initialized\n");
}


