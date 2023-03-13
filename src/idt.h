#ifndef IDT_H
#define IDT_H

#include <stdint.h>

void idt_init();
void exception_handler();
void idt_set_gate(uint8_t num, void* base,
		  uint16_t sel, uint8_t flags);
#endif
