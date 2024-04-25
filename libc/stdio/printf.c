#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<stdarg.h>
#include<limits.h>

static bool print(const char * data, size_t size) {
    const unsigned char * bytes = (const unsigned char *) data;
    for(size_t i = 0; i < size; i++) {
	if(bytes[i] == '\0')
		return true;
	if(bytes[i] == '\n') {
		new_line();
		continue;
	}
	if(bytes[i] == '\t') {
		new_tab();
		continue;
	}
        if(putchar(bytes[i]) == EOF) {
            return false;
        }
    }
    return true;
}

void int_to_string(int num, char *buffer) {
    int i = 0;
    bool is_negative = false;

    // Handle negative numbers
    if (num < 0) {
        is_negative = true;
        num = -num;
    }

    // Convert each digit of the number to characters
    do {
        buffer[i++] = '0' + num % 10;
        num /= 10;
    } while (num != 0);

    // Add negative sign if necessary
    if (is_negative) {
        buffer[i++] = '-';
    }

    // Null-terminate the string
    buffer[i] = '\0';

    // Reverse the string
    int j = 0;
    if (is_negative) {
        j = 1; // Skip negative sign
    }
    while (j < i - j - 1) {
        char temp = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = temp;
        j++;
    }
}


int printf(const char* restrict format, ...) {
    va_list parameters;
    va_start(parameters, format);
    int written = 0;
    while(*format != '\0') {
        size_t maxrem = INT_MAX - written;
        if(format[0] != '%' || format[1] == '%') {
            if(format[0] == '%') {
                format++;
            }
            size_t amount = 1;
            while(format[amount] && format[amount] != '%' && format[amount] != '\\' ) 
                amount++;
            if(maxrem < amount) {
                // return error
                return -1;
            }
            if(!print(format, amount)) {
                // return error
                return -1;
            }
            format += amount;
            written += amount;
            continue;
        }
        
        const char* format_begin_at = format++;
        if(format_begin_at[0] == '\\' && format[0] == 'n') {
		new_line();
	} else if(format_begin_at[0] == '%' && format[0] == 'c') {
            char c = (char) va_arg(parameters, int);
            if(!maxrem)
                return -1;
            if(!print(&c, 1))
                return -1;
            written++;
	    format++;
        } else if (format_begin_at[0] == '%' && format[0] == 's') {
            const char* str = va_arg(parameters, const char*);
            size_t len = strlen(str);
            if(maxrem < len){
                return -1;
	    }
            if(!print(str, len)) {
                return -1;
	    }
            written += len;
	    format++;
        } else if(format_begin_at[0] == '%' && format[0] == 'd') {
		int num = va_arg(parameters, int);
		char buffer[20];
		int_to_string(num, buffer);
		if(!print(buffer, sizeof(buffer))) {
				return -1;
		}
		written += sizeof(buffer);
		format++;
	} else {
            size_t len = strlen(format_begin_at);
            if (maxrem < len) {
			return -1;
		}
	if (!print(format_begin_at, len))
		return -1;
	written += len;
	format += len;
        }
    }
    va_end(parameters);
    return written;
}
