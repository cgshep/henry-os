#include "idt.h"
#include "string.h"
#include "tty.h"
#define MAX_IDT_ENTRIES 256

/* 
 * Defines an IDT entry. We use the packed attribute
 * force GCC to add no extra padding and other
 * data between each struct field.
 */
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

idt_entry idt[MAX_IDT_ENTRIES];
idt_descriptor idtd;

extern void *isr_stub_table[];

void exception_handler()
{
    terminal_write_string("WE INTERRUPTED!\n");
}

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

void idt_init()
{
    idtd.limit = (sizeof(idt_entry) * 256) - 1;
    idtd.base = (uint32_t)&idt[0];

    memset(&idt, 0, sizeof(idt_entry) * 256);

    for(size_t i = 0; i < 32; i++) {
	idt_set_gate(i, isr_stub_table[i], 0x08, 0x8E);
    }

    asm volatile ("lidt %0" :: "m"(idtd));
}
