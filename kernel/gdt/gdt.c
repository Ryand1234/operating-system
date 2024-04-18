#include<string.h>
#include "gdt.h"
#include<stdio.h>
struct gdtdesc kgdt[GDTSIZE];
struct gdtr kgdtr;


void read_gdtr(uint32_t* tgdtr) {
    asm("sgdt %0" : "=m" (*tgdtr));
}
void gdt_init(void) {

	create_gdt_descriptor(0x0, 0x0, 0x0, 0x0, &kgdt[0]);
	create_gdt_descriptor(0x0, 0xFFFFF, 0x9A, 0x0C, &kgdt[1]);
	create_gdt_descriptor(0x0, 0xFFFFF, 0x92, 0x0C, &kgdt[2]);
	create_gdt_descriptor(0x0, 0xFFFFF, 0xFA, 0x0C, &kgdt[3]);
	create_gdt_descriptor(0x0, 0xFFFFF, 0xF2, 0x0C, &kgdt[4]);   
//    	create_gdt_descriptor(0x0, 0xFFFFF, 0xF2, 0x0D, &kgdt[5]);   
	//create_gdt_descriptor(0x0, 0x0, 0xF7, 0x0D, &kgdt[6]); 
	kgdtr.limit = GDTSIZE*8;
	kgdtr.base = GDT_BASE;
	memcpy((char*) kgdtr.base, (char *) kgdt, kgdtr.limit);
	asm("lgdtl (kgdtr)");
	asm("movw $0x10, %ax \n \
		movw %ax, %ds \n \
		movw %ax, %es \n \
		movw %ax, %fs \n \
		movw %ax, %gs \n \
		ljmp $0x08, $next \n \
		next: \n	");
}

void create_gdt_descriptor(uint32_t base, uint32_t limit, uint8_t access, uint8_t flags, struct gdtdesc *desc) {
	desc->limit0_15 = (limit & 0xffff);
	desc->base0_15 = (base & 0xffff);
	desc->base16_23 = (base & 0xff0000) >> 16;
	desc->access_bytes = access;
	desc->limit16_19 = (limit & 0xf0000) >> 16;
	desc->flags = (flags & 0xf);
	desc->base24_31 = (base & 0xff000000) >> 24;
	return;
}
