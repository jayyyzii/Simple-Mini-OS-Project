all:
	mkdir -p build iso/boot/grub
	nasm -f elf64 kernel/boot.s -o build/boot.o
	gcc -c kernel/kernel.c -o build/kernel.o -ffreestanding -m64 -nostdlib -fno-pie
	ld -T kernel/linker.ld -o build/kernel.bin build/boot.o build/kernel.o

	cp build/kernel.bin iso/boot/
	cp boot/grub/grub.cfg iso/boot/grub/

	grub-mkrescue -o minios.iso iso

run:
	qemu-system-x86_64 -cdrom minios.iso -boot d -m 256M


clean:
	rm -rf build iso minios.iso
