#ifndef ARCH_DEVICES_KEYBOARD_H
#define ARCH_DEVICES_KEYBOARD_H
#include<arch/i386/x86.h>
#include<kernel/io.h>

void keyboard_handler(struct regs *r);
void pause();
#endif
