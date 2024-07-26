#include<kernel/io.h>
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
