#include<string.h>
#include "gdt.h"
#include<stdio.h>
struct gdtdesc kgdt[5];
struct gdtr kgdtr;


void read_gdtr(uint32_t* tgdtr) {
    asm("sgdt %0" : "=m" (*tgdtr));
}

void load_gdt(struct gdtr *gdtr) {
	asm volatile("lgdt %0" : : "m" (*gdtr));
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
