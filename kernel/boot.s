%define ALIGN   (1<<0)
%define MEMINFO (1<<1)
%define FLAGS   (ALIGN | MEMINFO)
%define MAGIC   0x1BADB002
%define CHECKSUM -(MAGIC + FLAGS)

section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

section .text
global _start
extern kernel_main

_start:
    call kernel_main
    cli
.hang:
    hlt
    jmp .hang
