#include<string.h>

void* memmove(void* desptr, const void* srcptr, size_t size) {
    unsigned char* dest = (unsigned char*) desptr;
    const unsigned char* src = (const unsigned char*) srcptr;
    if(dest < src) {
        for(size_t i = 0; i < size; i++) {
            dest[i] = src[i];
        }
    } else {
        for(size_t i = size; i > 0; i--) {
            dest[i-1] = src[i-1];
        }
    }
    return desptr;
}
