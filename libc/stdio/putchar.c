#include<stdio.h>

//#if defined(__is_libk)
#include<kernel/tty.h>
//#endif

int putchar(int ic) {
#if defined(__is_libk)
    char c = (char) ic;
    terminal_write(&c, sizeof(c));
#else
    // implement stdio system call and write procedure
#endif
    return ic;
}

void new_line() {
#if defined(__is_libk)
	terminal_new_line();
#else
	// implement somthing
#endif
}

void new_tab() {
#if defined(__is_libk)
	terminal_new_tab();
#else
	//implement somthing
#endif
}
