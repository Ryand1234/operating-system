#include<stdio.h>
#include<kernel/tty.h>
//#include<arch/i386/x86.h>
#include<mmu/vmm.h>
#include<mmu/alloc.h>
void divide_by_zero(void);

void kernel_main(void) {
    terminal_initialize();
    init_idt();
   setup_isr();
 //   isrs_install();
    printf("Terminal initialization compelete\n");
    printf("Tesing formating, %c \t%d %s\n", 'Q', 1234, "Hello world from format");
    memory_init(20);
    
    char* test = (char*) kmalloc(20);
    printf("Memory Address %d\n", test);
    divide_by_zero();
    printf("DONE");
}

void divide_by_zero(void) {
	//asm("hlt");
	int i = 10/0;

	printf("%d", i);
}
