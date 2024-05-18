#include<arch/i386/interrupts.h>
#include<stdio.h>

extern void _asm_int_0();
extern void _asm_int_1();
extern void isr0();

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

void *irq_routines[16] ={
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

void fault_handler(struct regs *r)
{
	printf("ERROR everywhere\n");
    if (r->int_no < 32)
    {
	    printf("ERROR here\n");
	    asm("hlt");
    } else {
	    printf("ERROR there\n");
	    asm("hlt");
    }
}

void setup_isr(void) {
	create_idt_descriptor(0x08, (uint32_t)isr0, INTGATE, 0);
	create_idt_descriptor(0x08, (uint32_t)isr0, INTGATE,1);
	create_idt_descriptor(0x08, (uint32_t)isr0, INTGATE,2);
	create_idt_descriptor(0x08, (uint32_t)isr0, INTGATE,3);
	create_idt_descriptor(0x08, (uint32_t)isr0, INTGATE,4);
	create_idt_descriptor(0x08, (uint32_t)isr0, INTGATE,5);
	create_idt_descriptor(0x08, (uint32_t)isr0, INTGATE,6);
}

void irq_install_handler(int irq, void (*handler)(struct regs *r)){
    irq_routines[irq] = handler;
}
void irq_uninstall_handler(int irq){
    irq_routines[irq] = 0;
}

void irq_install(){
	create_idt_descriptor(0x08, (unsigned)irq0, INTGATE,32);
	create_idt_descriptor(0x08, (unsigned)irq1, INTGATE,33);
	create_idt_descriptor(0x08, (unsigned)irq2, INTGATE,34);
	create_idt_descriptor(0x08, (unsigned)irq3, INTGATE,35);
	create_idt_descriptor(0x08, (unsigned)irq4, INTGATE,36);
	create_idt_descriptor(0x08, (unsigned)irq5, INTGATE,37);
	create_idt_descriptor(0x08, (unsigned)irq6, INTGATE,38);
	create_idt_descriptor(0x08, (unsigned)irq7, INTGATE,39);
	create_idt_descriptor(0x08, (unsigned)irq8, INTGATE,40);
	create_idt_descriptor(0x08, (unsigned)irq9, INTGATE,41);
	create_idt_descriptor(0x08, (unsigned)irq10, INTGATE,42);
	create_idt_descriptor(0x08, (unsigned)irq11, INTGATE,43);
	create_idt_descriptor(0x08, (unsigned)irq12, INTGATE,44);
	create_idt_descriptor(0x08, (unsigned)irq13, INTGATE,45);
	create_idt_descriptor(0x08, (unsigned)irq14, INTGATE,46);
	create_idt_descriptor(0x08, (unsigned)irq15, INTGATE,47);
}

void irq_handler(struct regs *r){
    void (*handler)(struct regs *r);

    handler = irq_routines[r->int_no - 32];
    if (handler)
    {
        handler(r);
    }

    // * If the IDT entry that was invoked was greater than 40
   // *  (meaning IRQ8 - 15), then we need to send an EOI to
    // *  the slave controller 
    if (r->int_no >= 40)
    {
        outb(0xA0, 0x20);
    }

    // * In either case, we need to send an EOI to the master
    // *  interrupt controller too 
    outb(0x20, 0x20);
}


