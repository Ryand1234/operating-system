#include<stdio.h>
#include<kernel/tty.h>
#include<arch/i386/x86.h>
#include<arch/i386/interrupts.h>
#include<arch/devices/keyboard.h>
#include<mmu/vmm.h>
#include<mmu/alloc.h>
void divide_by_zero(void);

int timer_ticks = 0;
void timer_handler(struct regs *r){
    /* Increment our 'tick count' */
    timer_ticks++;

    //ONE SECOND = 18 TICKS
}
void kernel_main(void) {
    terminal_initialize();
    gdt_init();
    init_idt();
    setup_isr();
    irq_install_handler(0, timer_handler);
    irq_install_handler(1,keyboard_handler);
    init_pic();
    irq_install();
    outb(0x21, inb(0x21) & ~0x02);
    printf("Terminal initialization compelete\n");
    printf("Tesing formating, %c \t%d %s\n", 'Q', 1234, "Hello world from format");
    memory_init(20);  
  /*  char* test = (char*) kmalloc(20);
    printf("Memory Address %d\n", test);
    char* test1 = (char*) kmalloc(50);
    printf("Memory Address for test1: %d\n", test1);
    kfree(test);
    char* test2= (char*) kmalloc(20);
    printf("Memory Address for test2: %d\n", test2);*/
    while(inb(0x60) & 0x80){};
//    divide_by_zero();
    asm volatile("sti");
    printf("DONE\n");
    while(1) {
    }
}

void divide_by_zero(void) {
	//asm("hlt");
	int i = 10/0;

	printf("%d", i);
}
