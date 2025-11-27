#include "syscall.h"

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define WHITE_ON_BLACK 0x0F

void clear_screen() {
    volatile char* vga = (volatile char*)VGA_ADDRESS;
    for (int i = 0; i < VGA_WIDTH * 25; i++) {
        vga[i * 2] = ' ';
        vga[i * 2 + 1] = WHITE_ON_BLACK;
    }
}

void kernel_main() {
    clear_screen();
    sys_print("MiniOS syscall active!");
    while (1) { }
}
