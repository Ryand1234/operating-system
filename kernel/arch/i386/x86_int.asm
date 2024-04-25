%macro SAVE_REGS 0
	pushad
	push ds
	push es
	push fs
	push gs
	push ebx
	mov bx, 0x10
	mov ds, bx
	pop ebx
%endmacro

%macro RESTORE_REGS 0
	pop gs
	pop fs
	pop es
	pop ds
	popad
%endmacro

%macro INTERRUPT 1
global _asm_int_%1
_asm_int_%1:
	SAVE_REGS
	push %1
	call isr_default_int
	pop eax
	mov al, 0x20
	out 0x20, al
	RESTORE_REGS
	iret
%endmacro

extern isr_default_int

INTERRUPT 1
INTERRUPT 0
