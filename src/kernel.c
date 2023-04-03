#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "tty.h"
#include "string.h"
#include "idt.h"
#include "pic.h"
#include "keyboard.h"
#include "memman.h"
#include "console.h"
#include "demo.h"

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "Must be compiled with the i386-elf compiler"
#endif

#ifdef DEBUG_INTERRUPTS
static void debug_test_software_interrupts()
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

static inline void loop() {
    for (;;){}
}

void kmain()
{
    idt_init();
    pic_init();
    terminal_init();
    terminal_print_banner();
    keyboard_init();
    console_init();
    memman_init();
    enable_interrupts();
#ifdef DEBUG_DEMO_ASCON
    int res = demo_ascon_aead();
    puts("ASCON res: ");
    terminal_write_int(res);
    puts("\n");
#endif
#ifdef DEBUG_INTERRUPTS
    debug_test_software_interrupts();
#endif
    loop();
}
