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
	iret
%endmacro

extern fault_handler

INTERRUPT 1
INTERRUPT 0


global isr0


%macro ISR_NOCODE 1
	isr%1:
		cli
		push byte 0
		push byte %1
		jmp isr_common_stub
%endmacro



;Divide By Zero Exception
ISR_NOCODE 0

isr_common_stub:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10   ; Load the Kernel Data Segment descriptor!
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp   ; Push us the stack
    push eax
    mov eax, fault_handler
    call eax       ; A special call, preserves the 'eip' register
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8     ; Cleans up the pushed error code and pushed ISR number
    iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!
	
