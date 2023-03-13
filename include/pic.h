#ifndef PIC_H
#define PIC_H

#include <stdint.h>

void enable_interrupts();
void pic_init();
void irq_set_mask(uint8_t irq_line);
void irq_clear_mask(uint8_t irq_line);
void irq_handler();
uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t val);

#endif
