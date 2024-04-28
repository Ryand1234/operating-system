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
	hlt
	SAVE_REGS
	push %1
	push eax
	call fault_handler
	pop eax
	mov al, 0x20
	out 0x20, al
	RESTORE_REGS
	add esp, 8
	ret
%endmacro

extern fault_handler

INTERRUPT 1
INTERRUPT 0

