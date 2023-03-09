OUT_DIR:=out
ISO_DIR:=$(OUT_DIR)/iso
OBJ:=$(OUT_DIR)/obj
BIN:=$(OUT_DIR)/bin
OS_OUT:=$(OUT_DIR)/dist
CC:=i686-elf-gcc
ASM:=nasm

.PHONY=all
all: dirs kernel link create-grub
	@echo "Done!"

dirs:
	@if [ ! -d $(OBJ) ]; then \
		mkdir -p $(OBJ); \
	fi
	@if [ ! -d $(BIN) ]; then \
		mkdir -p $(BIN); \
	fi
	@if [ ! -d $(OS_OUT) ]; then \
		mkdir -p $(OS_OUT); \
	fi
	@if [ ! -d $(ISO_DIR) ]; then \
		mkdir -p $(ISO_DIR); \
	fi

boot: dirs boot.asm
	@echo "<<< Compiling boot program >>>"
	$(ASM) -felf32 boot.asm -o $(OBJ)/boot.o

kernel: dirs kernel.c
	@echo "<<< Compiling kernel >>>"
	$(CC) -c kernel.c -o $(OBJ)/kernel.o -std=gnu99 -ffreestanding -O0 -Wall -Wextra -g

link: dirs boot kernel
	$(CC) -T linker.ld -o $(BIN)/henryos.bin -ffreestanding -O0 -nostdlib $(OBJ)/boot.o $(OBJ)/kernel.o -lgcc

create-grub: link
	@if grub-file --is-x86-multiboot $(BIN)/henryos.bin; then \
		echo "<Passed> henryos.bin is an x86 multiboot binary.";\
	else \
		echo "<Failed> henryos.bin is not an x86 multiboot binary!";\
	fi
	@if [ -d $(ISO_DIR) ]; then \
		echo "Removing existing ISO directory...";\
		rm -r $(ISO_DIR);\
	fi
	@mkdir -p $(ISO_DIR)/boot/grub
	@cp $(BIN)/henryos.bin $(ISO_DIR)/boot/
	@cp grub.cfg $(ISO_DIR)/boot/grub/
	grub-mkrescue -o $(OS_OUT)/henryos.iso $(ISO_DIR)
	@echo "Successfully created ISO file."

run:
	qemu-system-i386 -drive format=raw,file=$(OS_OUT)/henryos.iso

clean:
	@if [ -d $(OUT_DIR) ]; then \
		rm -r $(OUT_DIR); \
	fi

