#ifndef ARCH_I386_INTERRUPTS_H
#define ARCH_I386_INTERRUPTS_H
#include<arch/i386/x86.h>
void setup_isr(void);

void irq_install_handler(int irq, void (*handler)(struct regs *r));
void irq_uninstall_handler(int irq);
void irq_remap(void);
void irq_install();
void irq_handler(struct regs *r);

#endif
