# Declare constant for multiboot headers 
.set ALIGN, 1 << 0
.set MEMINFO, 1<<1
.set FLAGS, ALIGN | MEMINFO
.set MAGIC, 0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

# Declare multiboot headers
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Reserve stack for initial thread
.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:


#kernel entry point
.section .text
.global _start
.type _start, @function
_start:
    movl $stack_top, %esp
    call _init
    call kernel_main
1: hlt
    jmp 1b

.size _start, . - _start
