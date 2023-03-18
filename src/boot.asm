MBALIGN  equ  1 << 0            ; align loaded modules on page boundaries
MEMINFO  equ  1 << 1            ; provide memory map
MBFLAGS  equ  MBALIGN | MEMINFO ; this is the Multiboot 'flag' field
MAGIC    equ  0x1BADB002        ; 'magic number' lets bootloader find the header
CHECKSUM equ -(MAGIC + MBFLAGS)   ; checksum of above, to prove we are multiboot
	;;
	section .multiboot
align 4
	dd MAGIC
	dd MBFLAGS
	dd CHECKSUM
	
section .bss
	align 16

	global stack_bottom
stack_bottom:
	resb 4000h 		; 16KB
	global stack_top
stack_top:

	global heap_bottom
heap_bottom:
	resb 8000h		; 32KB
	global heap_top
heap_top:

	section .text
	global _start:function (_start.end - _start)
_start:
	mov esp, stack_top
	
setup_gdt_asm:
	cli
	lgdt [gdt_descriptor]
	mov eax, cr0
	or eax, 1
	mov cr0, eax
	jmp CODE_SEG:start_protected_mode

start_protected_mode: 
	mov ax, DATA_SEG
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	;;  Call our kernel's main function
	extern kmain
	call kmain
	
	cli
.hang:	hlt
	jmp .hang
_start.end:

gdt_start:
	dd 0x0
	dd 0x0
gdt_code: 
	dw 0xffff    ; segment length, bits 0-15
	dw 0x0       ; segment base, bits 0-15
	db 0x0       ; segment base, bits 16-23
	db 10011010b ; flags (8 bits)
	db 11001111b ; flags (4 bits) + segment length, bits 16-19
	db 0x0       ; segment base, bits 24-31

gdt_data:
	dw 0xffff
	dw 0x0
	db 0x0
	db 10010010b
	db 11001111b
	db 0x0
gdt_end:

gdt_descriptor:
	dw gdt_end - gdt_start - 1 
	dd gdt_start

CODE_SEG equ gdt_code - gdt_start ; 0x8
DATA_SEG equ gdt_data - gdt_start ; 0x10
