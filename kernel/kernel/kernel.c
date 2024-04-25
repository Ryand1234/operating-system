#include<stdio.h>
#include<kernel/tty.h>

void kernel_main(void) {
    terminal_initialize();
    printf("Terminal initialization compelete\n");
    printf("Tesing formating, %c \t%d %s", 'Q', 1234, "Hello world from format");
}
