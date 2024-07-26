#ifndef KERNEL_IO_H
#define KERNEL_IO_H
#include<stdint.h>
void outb(uint32_t , uint8_t );

uint8_t inb(uint32_t );

#endif