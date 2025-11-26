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

void print(const char* str) {
    volatile char* vga = (volatile char*)VGA_ADDRESS;
    int i = 0;
    while (str[i]) {
        vga[i * 2] = str[i];
        vga[i * 2 + 1] = WHITE_ON_BLACK;
        i++;
    }
}

void kernel_main() {
    clear_screen();
    print("hello world, lorem ipsum, gacor kang");
    while (1) { }
}
