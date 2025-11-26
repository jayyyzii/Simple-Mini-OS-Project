// syscall.c
#include "syscall.h"
#include "driver.h"

void init_syscall() { }

void sys_print(const char *s) {
    driver_write(s);
}
