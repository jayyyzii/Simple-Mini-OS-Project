#include "syscall.h"

#define VGA_ADDRESS 0xB8000
#define WHITE_ON_BLACK 0x0F

void kprint(const char* str) {
    volatile char* vga = (volatile char*)VGA_ADDRESS;
    int i = 0;
    while (str[i]) {
        vga[i * 2] = str[i];
        vga[i * 2 + 1] = WHITE_ON_BLACK;
        i++;
    }
}

void sys_print(const char* str) {
    kprint(str);
}
