void kernel_main() {
    char *vga = (char*)0xb8000;
    vga[0] = 'O';
    vga[1] = 0x0F;
    vga[2] = 'S';
    vga[3] = 0x0F;

    while(1) { }
}
