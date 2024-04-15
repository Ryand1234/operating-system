#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<stdarg.h>
#include<limits.h>

static bool print(const char * data, size_t size) {
    const unsigned char * bytes = (const unsigned char *) data;
    for(size_t i = 0; i < size; i++) {
        if(putchar(bytes[i]) == EOF) {
            return false;
        }
    }
    return true;
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
            while(format[amount] && format[amount] != '%') 
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
        if(*format == '%' && *format_begin_at == 'c') {
            char c = (char) va_arg(parameters, int);
            if(!maxrem)
                return -1;
            if(!print(&c, sizeof(c)))
                return -1;
            written++;
        } else if (*format == '%' && *format_begin_at == 's') {
            const char* str = va_arg(parameters, const char*);
            size_t len = strlen(str);
            if(maxrem < len){
                return -1;
	    }
            if(!print(str, len)) {
                return -1;
	    }
            written += len;
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
        format++;
    }
    va_end(parameters);
    return written;
}
