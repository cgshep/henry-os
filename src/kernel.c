#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "tty.h"
#include "string.h"
 
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


void kernel_main()
{
    terminal_initialize();
    terminal_print_banner();
    terminal_write_string(">> ");
}
