#ifndef IDT_H
#define IDT_H

#include <stdint.h>

#define INTERRUPT_GATE              0x8e00
#define TRAP_GATE                   0x8f00

// ------------------------------------------------------------------------------------------------
void idt_init();
void IdtSetHandler(uint8_t index, uint16_t type, void (*handler)());

#endif
