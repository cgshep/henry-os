OUT_DIR:=out
SRC_DIR:=src
ISO_DIR:=$(OUT_DIR)/iso
OBJ:=$(OUT_DIR)/obj
BIN:=$(OUT_DIR)/bin
OS_OUT:=$(OUT_DIR)/dist
CC:=i686-elf-gcc
ASM:=nasm
CFLAGS:=$(CFLAGS) -std=gnu11 -ffreestanding -O0 -Wall -Wextra
INCLUDE_DIRS:=include
INC:=-I$(INCLUDE_DIRS)/

OBJS=$(OBJ)/tty.o \
$(OBJ)/kernel.o \
$(OBJ)/boot.o \
$(OBJ)/string.o \
$(OBJ)/idt.o \
$(OBJ)/interrupts.o \
$(OBJ)/pic.o \
$(OBJ)/keyboard.o \
$(OBJ)/console.o \
$(OBJ)/strtok.o \
$(OBJ)/calculator.o \
$(OBJ)/demo.o \
$(OBJ)/permutations.o \
$(OBJ)/core.o \
$(OBJ)/decrypt.o \
$(OBJ)/encrypt.o \
$(OBJ)/memman.o

.PHONY=all

all: dirs henryos.bin create-grub
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

$(OBJ)/%.o: $(SRC_DIR)/%.c 
	$(CC)  $(CFLAGS)  -c $< -o $@ $(INC)

$(OBJ)/%.o: $(SRC_DIR)/crypto/%.c
	$(CC)  $(CFLAGS)  -c $< -o $@ $(INC)

$(OBJ)/%.o: $(SRC_DIR)/apps/%.c
	$(CC)  $(CFLAGS)  -c $< -o $@ $(INC)

$(OBJ)/%.o: $(SRC_DIR)/%.asm
	$(ASM) -felf32 $< -o $@

henryos.bin: $(OBJS)
	$(CC) -T linker.ld -o $(BIN)/$@ -ffreestanding -O2 -nostdlib -lgcc $(OBJS)

create-grub: henryos.bin
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
	qemu-system-i386 -d pcall,cpu_reset,int -no-reboot \
	-drive format=raw,file=$(OS_OUT)/henryos.iso

clean:
	@if [ -d $(OUT_DIR) ]; then \
		rm -r $(OUT_DIR); \
	fi

