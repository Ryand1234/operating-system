#include<stdbool.h>
#include<stddef.h>
#include<stdint.h>
#include<string.h>

#include<kernel/tty.h>
#include "vga.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 100;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;
static uint16_t* terminal_memory;

void terminal_initialize(void) {
    terminal_column = 0;
    terminal_row = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = VGA_MEMORY;
    for(size_t y = 0; y < VGA_HEIGHT; y++) {
        for(size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
	    terminal_memory[y] = VGA_WIDTH-1;
        }
    }
}

void set_color(uint8_t color) {
    terminal_color = color;
}

bool terminal_can_backspace(void) {
	if(terminal_column == 0 && terminal_row == 0)
		return false;
	if(terminal_column > 0 || terminal_row >= 0)
		return true;
	return true;
}

void terminal_backspace(void) {
	if(terminal_can_backspace() == false)
		return;
	if(terminal_column == 0)
	{
		terminal_row--;
		terminal_column = terminal_memory[terminal_row];
	} else {
		terminal_column--;
	}
	terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
}

void terminal_new_line(void) {
	terminal_column = 0;
	terminal_row++;
}

void terminal_new_tab(void) {
	terminal_column += 4;
	if(terminal_column >= VGA_WIDTH) {
		terminal_row++;
		terminal_column = (terminal_column % VGA_WIDTH);
	}
}

void terminal_putentryat(unsigned char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    terminal_memory[y] = x;
    terminal_buffer[index] = vga_entry(c, color);
}

void terminal_scroll(int line) {
    int *loop;
    char c;
    
	for(loop = line * (VGA_WIDTH * 2) + 0xB8000; loop < VGA_WIDTH * 2; loop++) {
		c = *loop;
		*(loop - (VGA_WIDTH * 2)) = c;
	}
}

void terminal_delete_last_line() {
	int x, *ptr;
 
	for(x = 0; x < VGA_WIDTH * 2; x++) {
		ptr = 0xB8000 + (VGA_WIDTH * 2) * (VGA_HEIGHT - 1) + x;
		*ptr = 0;
	}
}

void terminal_putchar(char c) {
    int line;
    unsigned char uc = c;
    
    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    if(++terminal_column == VGA_WIDTH) {
	terminal_memory[terminal_row] = VGA_WIDTH-1;
        terminal_column = 0;
        if(++terminal_row == VGA_HEIGHT) {

                for(line = 1; line < VGA_HEIGHT - 1; line++) {
                    terminal_scroll(line);
                }
                terminal_delete_last_line();
                terminal_row = VGA_HEIGHT - 1;
        }
    }
}

void terminal_write(const char* data, size_t size) {
    for(size_t i = 0; i < size; i++) {
        terminal_putchar(data[i]);
    }
}

void terminal_writestring(const char* data) {
    terminal_write(data, strlen(data));
}
