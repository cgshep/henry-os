#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "tty.h"
#include "string.h"
#include "idt.h"
#include "pic.h"
#include "timer.h"
#include "keyboard.h"
#include "console.h"

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "Must be compiled with the i386-elf compiler"
#endif

uint64_t get_time()
{
    uint32_t a, b;
    // Use RDTSC
    asm volatile ("rdtsc" : "=a"(a), "=d"(b));
    return ((uint64_t) b) | (((uint64_t) a) << 32);
}

#ifdef DEBUG_INTERRUPTS
void debug_test_software_interrupts()
{
    terminal_write_string(">> Trying `int 0x01`...");
    asm("int $0x01":);
    terminal_write_string(">> Trying `int 0x10`...");
    asm("int $0x10":);
    terminal_write_string(">> Trying `int 0x04`...");
    asm("int $0x04":);
    terminal_write_string(">> Trying `int 0x00`...");
    asm("int $0x00":);
    terminal_write_string(">> Trying `int 0x03`...");
    asm("int $0x03":);
    terminal_write_string(">> Trying `int 0x11`...");
    asm("int $0x11":);
    terminal_write_string(">> Trying `int 0x19`...");
    asm("int $0x19":);    
}
#endif

#ifdef DEBUG_MEMM
extern volatile uint32_t stack_bottom;
extern volatile uint32_t stack_top;
extern volatile uint32_t heap_bottom;
extern volatile uint32_t heap_top;

void debug_memory_management()
{
    puts("\n");
    puts("kernel.c stack_bottom: ");
    terminal_write_int((uint32_t)&stack_bottom);
    puts(", stack_top: ");
    terminal_write_int((uint32_t)&stack_top);
    puts(", stack size: ");
    terminal_write_int((uint32_t)&stack_top -
		       (uint32_t)&stack_bottom);

    puts("\n");
    puts("kernel.c heap_bottom: ");
    terminal_write_int((uint32_t)&heap_bottom);
    puts(", heap_top: ");
    terminal_write_int((uint32_t)&heap_top);
    puts(", heap size: ");
    terminal_write_int((uint32_t)&heap_top -
		       (uint32_t)&heap_bottom);
    puts("\n");
}
#endif

void loop() {
    for (;;){}
}

void kmain()
{
    idt_init();
    pic_init();
    timer_install();
    terminal_init();
    terminal_print_banner();
    keyboard_init();
    console_init();
    enable_interrupts();

#ifdef DEBUG_INTERRUPTS
    debug_test_software_interrupts();
#endif
#ifdef DEBUG_MEMM
    debug_memory_management();
#endif
    loop();
}
