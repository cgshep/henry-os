#include "idt.h"
#include "string.h"
#include "tty.h"

/* Defines an IDT entry */
typedef struct
{
    uint16_t base_low;
    uint16_t seg_sel;
    uint8_t reserved;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed)) idt_entry;


typedef struct
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_descriptor;

/* Declare an IDT of 256 entries. Although we will only use the
*  first 32 entries in this tutorial, the rest exists as a bit
*  of a trap. If any undefined IDT entry is hit, it normally
*  will cause an "Unhandled Interrupt" exception. Any descriptor
*  for which the 'presence' bit is cleared (0) will generate an
*  "Unhandled Interrupt" exception */
idt_entry idt[256];
idt_descriptor idtd;

void exception_handler()
{
    terminal_write_string("WE INTERRUPTED!\n");
}

/* Use this function to set an entry in the IDT. Alot simpler
*  than twiddling with the GDT ;) */
void idt_set_gate(uint8_t num, void* base,
		  uint16_t sel, uint8_t flags)
{
    /* The interrupt routine's base address */
    idt[num].base_low = (uint32_t)base & 0xFFFF;
    idt[num].base_high = (uint32_t)base >> 16;

    /* The segment or 'selector' that this IDT entry will use
    *  is set here, along with any access flags */
    idt[num].seg_sel = sel; // Kernel code segment
    idt[num].reserved = 0;
    idt[num].flags = flags;
}


extern void *isr_stub_table[];

void idt_init()
{
    idtd.limit = (sizeof(idt_entry) * 256) - 1;
    idtd.base = (uint32_t)&idt[0];

    memset(&idt, 0, sizeof(idt_entry) * 256);

    /* Add any new ISRs to the IDT here using idt_set_gate */
    for(size_t i = 0; i < 32; i++) {
	idt_set_gate(i, isr_stub_table[i], 0x08, 0x8E);
    }

    /* Load IDT */
    asm volatile ("lidt %0" :: "m"(idtd));
}
