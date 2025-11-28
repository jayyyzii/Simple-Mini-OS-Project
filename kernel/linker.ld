ENTRY(_start)

SECTIONS {
  . = 1M;

  .multiboot :
  {
    KEEP(*(.multiboot))
  }

  .text :
  {
    *(.text*)
  }

  .data :
  {
    *(.data*)
  }

  .bss :
  {
    *(.bss*)
  }
}
