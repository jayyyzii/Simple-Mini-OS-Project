# ======================================================
# MINI OS MAKEFILE - ROSSI TEAM
# ======================================================

# Tools (ARM cross-compiler)
CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy

# Directories
KERNEL_DIR = Kernel
SCHED_DIR = $(KERNEL_DIR)/scheduler
MEM_DIR = $(KERNEL_DIR)/memory
SYS_DIR = $(KERNEL_DIR)/sys
DRV_DIR = $(KERNEL_DIR)/driver
INCLUDE_DIR = $(KERNEL_DIR)/include

# Sources
SRCS = \
    $(KERNEL_DIR)/Kernel.c \
    $(SCHED_DIR)/scheduler.c \
    $(MEM_DIR)/memory.c \
    $(SYS_DIR)/syscall.c \
    $(DRV_DIR)/driver.c \
    start.S

# Objects
OBJS = $(SRCS:.c=.o)
OBJS := $(OBJS:.S=.o)

# Compile flags (only in compiler and assembler)
CFLAGS = -c -ffreestanding -nostdlib -I$(INCLUDE_DIR) -mcpu=arm926ej-s

# Linker flags (no -mcpu here)
LDFLAGS = -m armelf -Ttext=0x8000

# Build rules
all: kernel.bin

# Compile C source files
%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

# Compile assembly source files
%.o: %.S
	$(CC) $(CFLAGS) -o $@ $<

# Link all objects into application ELF
kernel.elf: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

# Convert ELF to binary
kernel.bin: kernel.elf
	$(OBJCOPY) -O binary kernel.elf kernel.bin
	@echo "Build complete. Output: kernel.bin"

# Run in QEMU emulator
run: kernel.bin
	qemu-system-arm -M versatilepb -m 128M -nographic -kernel kernel.bin

# Clean build files
clean:
	rm -f $(OBJS) kernel.elf kernel.bin
	@echo "Build files cleaned."

# ======================================================
# END
# ======================================================

