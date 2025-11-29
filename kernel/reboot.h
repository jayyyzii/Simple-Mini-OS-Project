// reboot.h
#ifndef REBOOT_H
#define REBOOT_H

static inline void reboot_qemu() {
    unsigned short data = 0x2000;
    unsigned short port = 0x604;

    __asm__ volatile (
        "outw %0, %1"
        :
        : "a"(data), "Nd"(port)
    );
}

#endif
