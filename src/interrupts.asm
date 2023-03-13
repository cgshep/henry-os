	[bits 32]
%macro isr_err_stub 1
isr_stub_%+%1:
	call exception_handler
	iret 
%endmacro
; if writing for 64-bit, use iretq instead
%macro isr_no_err_stub 1
isr_stub_%+%1:
	call exception_handler
	iret
%endmacro

extern exception_handler
isr_no_err_stub 0
isr_no_err_stub 1
isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_no_err_stub 6
isr_no_err_stub 7
isr_err_stub    8
isr_no_err_stub 9
isr_err_stub    10
isr_err_stub    11
isr_err_stub    12
isr_err_stub    13
isr_err_stub    14
isr_no_err_stub 15
isr_no_err_stub 16
isr_err_stub    17
isr_no_err_stub 18
isr_no_err_stub 19
isr_no_err_stub 20
isr_no_err_stub 21
isr_no_err_stub 22
isr_no_err_stub 23
isr_no_err_stub 24
isr_no_err_stub 25
isr_no_err_stub 26
isr_no_err_stub 27
isr_no_err_stub 28
isr_no_err_stub 29
isr_err_stub    30
isr_no_err_stub 31

global isr_stub_table
isr_stub_table:
%assign i 0 
%rep    32 
    dd isr_stub_%+i
%assign i i+1 
%endrep
	
extern irq_handler
%macro irq_stub_handler 1
irq_stub_%+%1:
	push esi
	mov esi, %1
	call irq_handler
	pop esi
	iret 
%endmacro

global irq_stub_table
irq_stub_table:
%assign i 0 
%rep    16
	dd irq_stub_%+i ; use DQ instead if targeting 64-bit
%assign i i+1 
%endrep

irq_stub_handler     0 ; Programmable Interrupt Timer Interrupt
irq_stub_handler     1 ; Keyboard Interrupt
irq_stub_handler     2 ; Cascade (used internally by the two PICs. never raised)
irq_stub_handler     3 ; COM2
irq_stub_handler     4 ; COM1
irq_stub_handler     5 ; LPT2
irq_stub_handler     6 ; Floppy Disk
irq_stub_handler     7 ; LPT1 (probably spurious)
irq_stub_handler     8 ; CMOS RTC
irq_stub_handler     9 ; Free for peripherals / legacy SCSI / NIC
irq_stub_handler    10 ; Free for peripherals / SCSI / NIC
irq_stub_handler    11 ; Free for peripherals / SCSI / NIC
irq_stub_handler    12 ; PS2 Mouse
irq_stub_handler    13 ; FPU / Coprocessor / Inter-processor
irq_stub_handler    14 ; Primary ATA Hard Disk
irq_stub_handler    15 ; Secondary ATA Hard Disk
