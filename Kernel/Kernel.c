// Kernel.c
#include "scheduler.h"
#include "memory.h"
#include "syscall.h"
#include "driver.h"

// Stub fungsi putchar, implementasi sederhana tanpa write()
void putchar(char c) {
    // Kosongkan dulu untuk build tanpa error
}

// Print menggunakan putchar jadi output serial/terminal
void print(const char *s) {
    while (*s) {
        putchar(*s++);
    }
}

void kernel_main() {
    print("=== MINI OS KERNEL START ===\n");

    init_memory();
    init_scheduler();
    init_driver();
    init_syscall();

    print("Kernel initialization complete.\n");

    while (1) { }
}

