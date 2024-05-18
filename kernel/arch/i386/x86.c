#include<string.h>
#include<arch/i386/x86.h>
#include<stdio.h>
struct gdtdesc kgdt[3];
struct gdtr kgdtr;
struct idtr kidtr;
struct idtdesc kidt[256];
uint8_t stack[STACKSIZE];


void read_gdtr(uint32_t* tgdtr) {
    asm("sgdt %0" : "=m" (*tgdtr));
}

void load_gdt(struct gdtr *gdtr) {
	asm volatile("lgdt %0" : : "m" (*gdtr));
}

void load_idt(struct idtr *idtr) {
	asm volatile("lidt %0" : : "m" (*idtr));
}

//extern void gdt_flush();

void gdt_init(void) {

	create_gdt_descriptor(0x0, 0x0, 0x0, 0x0, &kgdt[0]);
	create_gdt_descriptor(0x0, 0xFFFFFFFF, 0x9A, 0xCF, &kgdt[1]);
	create_gdt_descriptor(0x0, 0xFFFFFFFF, 0x92, 0xCF, &kgdt[2]);
	//create_gdt_descriptor(0x0, 0xFFFFFFFF, 0xFA, 0xCF, &kgdt[3]);
//	create_gdt_descriptor(0x0, 0xFFFFFFFF, 0xF2, 0xCF, &kgdt[4]);
//    	create_gdt_descriptor(0x0, 0xFFFFFFFF, 0xF2, 0xCF, &kgdt[5]);
//	create_gdt_descriptor(0x0, 0x0, 0xF7, 0x0D, &kgdt[6]);
	kgdtr.limit = (sizeof(struct gdtdesc) * 3) -1;
	kgdtr.base = (uint32_t)&kgdt;
	gdt_flush();
}

void create_gdt_descriptor(uint32_t base, uint32_t limit, uint8_t access, uint8_t flags, struct gdtdesc *desc) {
	if (desc == NULL) {
		printf("NULL");
	}
	desc->limit0_15 = (limit & 0xFFFF);
	desc->base0_15 = (base & 0xFFFF);
	desc->base16_23 = (base & 0xFF) >> 16;
	desc->access_bytes = access;
	desc->granularity = ((limit >> 16) ^ 0x0F);
	desc->granularity != (flags & 0xF0);
	desc->base24_31 = (base & 0xFF) >> 24;
	return;
}

void create_idt_descriptor(uint16_t select, uint32_t offset, uint8_t type, struct idtdesc * desc) {
	//printf("select: %d, offset: %d, type: %d\n", select, offset, type);
	desc->offset0_15 = (offset & 0xFFFF);
	desc->select = select;
	desc->access_gran = type;
	desc->alwaysZero = 0;
	desc->offset16_31 = (offset & 0xFFFF0000) >> 16;
	return;
}

extern void _asm_int_0();
extern void _asm_int_1();
extern void isr0();

void outb(uint32_t ad, uint8_t v)
{
	asm volatile("outb %%al, %%dx" :: "d" (ad), "a" (v));
}

uint8_t inb(uint32_t ad)
{
	uint8_t v; 
	asm volatile("inb %%dx, %%al" : "=a" (v) : "d" (ad));
	printf("IN: %d\n", v);
	return v;
}
void isr_default_int(int id)
{
	printf("default int %d", id);
	asm volatile("hlt");
}

void init_pic(void)
{
	outb(0x20, 0x11);
	outb(0xA0, 0x11);

	outb(0x21, 0x20);
	outb(0xA1, 0x28);

	outb(0x21, 0x04);
	outb(0xA1, 0x02);

	outb(0x21, 0x01);
	outb(0xA1, 0x01);

	outb(0x21, 0x0);
	outb(0xA1, 0x0);
	printf("PIC initialised\n");
}


extern void idt_load();

void init_idt(void) {

	kidtr.limit = (sizeof(struct idtdesc) * 256) - 1;
	kidtr.base = (uint32_t)&kidt;
	load_idt(&kidtr);
	printf("IDT initialized\n");
}

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
	create_idt_descriptor(0x08, (uint32_t)isr0, INTGATE, &kidt[0]);
	create_idt_descriptor(0x08, (uint32_t)isr0, INTGATE, &kidt[1]);
	create_idt_descriptor(0x08, (uint32_t)isr0, INTGATE, &kidt[2]);
	create_idt_descriptor(0x08, (uint32_t)isr0, INTGATE, &kidt[3]);
	create_idt_descriptor(0x08, (uint32_t)isr0, INTGATE, &kidt[4]);
	create_idt_descriptor(0x08, (uint32_t)isr0, INTGATE, &kidt[5]);
	create_idt_descriptor(0x08, (uint32_t)isr0, INTGATE, &kidt[6]);
}

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
void irq_install_handler(int irq, void (*handler)(struct regs *r)){
	printf("INSTALL %d\n", irq);
    irq_routines[irq] = handler;
}
void irq_uninstall_handler(int irq){
    irq_routines[irq] = 0;
}

void irq_install(){
	create_idt_descriptor(0x08, (unsigned)irq0, INTGATE, &kidt[32]);
	create_idt_descriptor(0x08, (unsigned)irq1, INTGATE, &kidt[33]);
	create_idt_descriptor(0x08, (unsigned)irq2, INTGATE, &kidt[34]);
	create_idt_descriptor(0x08, (unsigned)irq3, INTGATE, &kidt[35]);
	create_idt_descriptor(0x08, (unsigned)irq4, INTGATE, &kidt[36]);
	create_idt_descriptor(0x08, (unsigned)irq5, INTGATE, &kidt[37]);
	create_idt_descriptor(0x08, (unsigned)irq6, INTGATE, &kidt[38]);
	create_idt_descriptor(0x08, (unsigned)irq7, INTGATE, &kidt[39]);
	create_idt_descriptor(0x08, (unsigned)irq8, INTGATE, &kidt[40]);
	create_idt_descriptor(0x08, (unsigned)irq9, INTGATE, &kidt[41]);
	create_idt_descriptor(0x08, (unsigned)irq10, INTGATE, &kidt[42]);
	create_idt_descriptor(0x08, (unsigned)irq11, INTGATE, &kidt[43]);
	create_idt_descriptor(0x08, (unsigned)irq12, INTGATE, &kidt[44]);
	create_idt_descriptor(0x08, (unsigned)irq13, INTGATE, &kidt[45]);
	create_idt_descriptor(0x08, (unsigned)irq14, INTGATE, &kidt[46]);
	create_idt_descriptor(0x08, (unsigned)irq15, INTGATE, &kidt[47]);
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

int shift = 0;
unsigned char kbdus[256] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,
	0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+',
	'\b', '\t',
	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
	0,
	'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',
	'\"', '~', 0, '|',
	'Z', 'X', 'C', 'V', 'B', 'N', 'M',
	'<', '>', '?', 0, '*',
	0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0
	
};

void pause(){
	int i = 0;
	while(i < 100000000){
		i++;
	}
	while(inb(0x60) & 0x80){

	};
}
void keyboard_handler(struct regs *r){
    unsigned char scancode;
	printf("KEYBOARDd\n");
    scancode = inb(0x60);

    if (scancode & 0x80)
    {
		if(scancode == 0xAA)
			shift = 0;
    }
    else
    {
		if(scancode == 0x2a){shift = 1;return;}
	//	if(scancode == 0x1c){shell_enterpressed();return;}
/*		if(scancode == 0x0e){
			if(shell_can_backspace()){
				printchar(kbdus[scancode]);
			}
			shell_backspace();
			return;
		}*/
		if(shift){
			printf("%c",kbdus[scancode+90]);
//			shell_addchar(kbdus[scancode+90]);
		}else{
			printf("%c",kbdus[scancode]);
//			shell_addchar(kbdus[scancode]);
		}
    }
}

