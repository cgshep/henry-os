#ifndef TTY_H
#define TTY_H

#include <stddef.h>
#include <stdint.h>

#include "string.h"

#define TERMINAL_PROMPT ">> "
#define TERMINAL_PROMPT_LEN 3
#define VGA_MEMORY_LOCATION 0xB8000

enum vga_colour {
	VGA_COLOUR_BLACK = 0,
	VGA_COLOUR_BLUE = 1,
	VGA_COLOUR_GREEN = 2,
	VGA_COLOUR_CYAN = 3,
	VGA_COLOUR_RED = 4,
	VGA_COLOUR_MAGENTA = 5,
	VGA_COLOUR_BROWN = 6,
	VGA_COLOUR_LIGHT_GREY = 7,
	VGA_COLOUR_DARK_GREY = 8,
	VGA_COLOUR_LIGHT_BLUE = 9,
	VGA_COLOUR_LIGHT_GREEN = 10,
	VGA_COLOUR_LIGHT_CYAN = 11,
	VGA_COLOUR_LIGHT_RED = 12,
	VGA_COLOUR_LIGHT_MAGENTA = 13,
	VGA_COLOUR_LIGHT_BROWN = 14,
	VGA_COLOUR_WHITE = 15,
};

void terminal_new_cmd();
void terminal_cmd_delete_last_char();
void terminal_cmd_putch(char c);
void terminal_write_string(const char* data);
void terminal_write_hex(uint32_t byte);
void terminal_write(const char* data, size_t size);
void terminal_print_banner();
void terminal_put_char(char c);
void terminal_scroll_up();
void terminal_put_entry_at(char c, uint8_t colour, size_t x, size_t y);
void terminal_set_colour(uint8_t colour);
void terminal_init();

#endif
