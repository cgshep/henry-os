#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "tty.h"
#include "string.h"
#include "idt.h"
 
/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

uint64_t get_time()
{
    uint32_t a, b;
    // Use RDTSC
    asm volatile ("rdtsc" : "=a"(a), "=d"(b));
    return ((uint64_t) b) | (((uint64_t) a) << 32);
}

void exception_handler()
{
    terminal_write_string("WE INTERRUPTED!\n");
}

void kernel_main()
{
    idt_init();
    terminal_initialize();
    terminal_print_banner();
    terminal_write_string(">> Trying `int 0x01`...");
    asm("int $0x01");
    terminal_write_string(">> Trying `int 0x10`...");
    asm("int $0x10");
    terminal_write_string(">> Trying `int 0x04`...");
    asm("int $0x04");
    terminal_write_string(">> Trying `int 0x00`...");
    asm("int $0x00");
    terminal_write_string(">> Trying `int 0x03`...");
    asm("int $0x03");
    terminal_write_string(">> Trying `int 0x11`...");
    asm("int $0x11");
    terminal_write_string(">> Trying `int 0x19`...");
    asm("int $0x19");
    terminal_write_string(">> ALL DONE!");
}
