#include<string.h>
#include "x86.h"
#include<stdio.h>
struct gdtdesc kgdt[5];
struct gdtr kgdtr;
struct idtr kidtr;
struct idtdesc kidt[1];

void read_gdtr(uint32_t* tgdtr) {
    asm("sgdt %0" : "=m" (*tgdtr));
}

void load_gdt(struct gdtr *gdtr) {
	asm volatile("lgdt %0" : : "m" (*gdtr));
}

void load_idt(struct idtr *idtr) {
	asm volatile("lidt %0" : : "m" (*idtr));
}

void gdt_init(void) {

	create_gdt_descriptor(0x0, 0x0, 0x0, 0x0, &kgdt[0]);
	create_gdt_descriptor(0x0, 0xFFFF, 0x9A, 0xCF, &kgdt[1]);
	create_gdt_descriptor(0x0, 0xFFFF, 0x92, 0xCF, &kgdt[2]);
	create_gdt_descriptor(0x0, 0xFFFF, 0xFA, 0xCF, &kgdt[3]);
	create_gdt_descriptor(0x0, 0xFFFF, 0xF2, 0xCF, &kgdt[4]);
//    	create_gdt_descriptor(0x0, 0xFFFF, 0xF2, 0xCF, &kgdt[5]);
//	create_gdt_descriptor(0x0, 0x0, 0xF7, 0x0D, &kgdt[6]);
	kgdtr.limit = sizeof(kgdt) -1;
	kgdtr.base = (uint32_t)&kgdt;

	load_gdt(&kgdtr);
}

void create_gdt_descriptor(uint32_t base, uint32_t limit, uint8_t access, uint8_t flags, struct gdtdesc *desc) {
	if (desc == NULL) {
		printf("NULL");
	}
	desc->limit0_15 = (limit & 0xFFFF);
	desc->base0_15 = (base & 0xFFFF);
	desc->base16_23 = (base & 0xFF) >> 16;
	desc->access_bytes = access;
	desc->limit16_19 = (limit & 0xF) >> 16;
	desc->flags = (flags & 0xF);
	return;
}

void create_idt_descriptor(uint16_t select, uint32_t offset, uint16_t type, struct idtdesc * desc) {
	desc->offset0_15 = (offset & 0xFFFF);
	desc->select = select;
	desc->type = type;
	desc->offset16_31 = (offset & 0xFFFF0000) >> 16;
	return;
}

extern void _asm_int_0();
extern void _asm_int_1();


void outb(uint32_t ad, uint8_t v)
{
	asm volatile("outb %%al, %%dx" :: "d" (ad), "a" (v));
}

uint8_t inb(uint32_t ad)
{
	uint8_t v; 
	asm volatile("inb %%dx, %%al" : "=a" (v) : "d" (ad)); 
	return v;
}

void isr_default_int(int id)
{
	printf("default int %d", id);
}

void init_pic(void)
{
	outb(0x20, 0x11);
	outb(0xA0, 0x11);

	outb(0x21, 0x20);
	outb(0xA1, 0x70);

	outb(0x21, 0x04);
	outb(0xA1, 0x02);

	outb(0x21, 0x01);
	outb(0xA1, 0x01);

	outb(0x21, 0x0);
	outb(0xA1, 0x0);
}



void init_idt(void) {
	//int i;
/*	for(i = 0; i < IDTSIZE; i++)
	{
		init_idt_descriptor(0x01, (uint32_t)_asm_schedule, INTGATE, &kidt[i]);
	} */

	create_idt_descriptor(0x08, (uint32_t) _asm_int_1, INTGATE, &kidt[0]);
//	init_idt_descriptor(0x08, (uint32_t) _asm_init

	kidtr.limit = sizeof(kidt) - 1;
	kidtr.base = (uint32_t)&kidt;

	load_idt(&kidtr);
}
