#include<stdio.h>
#include<kernel/tty.h>
#include<arch/i386/x86.h>

void divide_by_zero(void);

void kernel_main(void) {
    terminal_initialize();
    init_idt();
    setup_isr();
    printf("Terminal initialization compelete\n");
    printf("Tesing formating, %c \t%d %s\n", 'Q', 1234, "Hello world from format");
    divide_by_zero();
}

void divide_by_zero(void) {
//	asm("hlt");
	int i = 10/0;
	printf("%d", i);
}
