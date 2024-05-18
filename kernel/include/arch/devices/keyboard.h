#ifndef ARCH_DEVICES_KEYBOARD_H
#define ARCH_DEVICES_KEYBOARD_H
#include<arch/i386/x86.h>

void keyboard_handler(struct regs *r);
void pause();
#endif
