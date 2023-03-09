#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
 
/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif
 
/* Hardware text mode colour constants. */
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

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
 
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_colour;
uint16_t* terminal_buffer;

uint64_t get_time()
{
    uint32_t a, b;
    // Use RDTSC
    asm volatile ("rdtsc" : "=a"(a), "=d"(b));
    return ((uint64_t) b) | (((uint64_t) a) << 32);
}

static inline uint8_t vga_entry_colour(enum vga_colour fg,
				      enum vga_colour bg) 
{
    return fg | bg << 4;
}
 
static inline uint16_t vga_entry(unsigned char uc,
				 uint8_t colour) 
{
    return (uint16_t) uc | (uint16_t) colour << 8;
}
 
size_t strlen(const char* str)
{
	size_t len = 0;
	while (str[len])
	    len++;
	return len;
}

void terminal_initialize() 
{
    terminal_row = 0;
    terminal_column = 0;
    terminal_colour = vga_entry_colour(VGA_COLOUR_WHITE,
				       VGA_COLOUR_GREEN);
    terminal_buffer = (uint16_t*) 0xB8000;
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
	for (size_t x = 0; x < VGA_WIDTH; x++) {
	    const size_t index = y * VGA_WIDTH + x;
	    terminal_buffer[index] = vga_entry(' ', terminal_colour);
	}
    }
}


void terminal_set_colour(uint8_t colour) 
{
    terminal_colour = colour;
}
 
void terminal_put_entry_at(char c, uint8_t colour, size_t x, size_t y) 
{
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, colour);
}

void terminal_scroll_up()
{
    // We need to redraw the terminal VGA buffer, minus
    // a whole line. Then we fill the last line as empty.
    for(size_t y = 0; y < VGA_HEIGHT; y++) {
	for(size_t x = 0; x < VGA_WIDTH; x++ ) {
	    const size_t index = (y * VGA_WIDTH) + x;
	    terminal_buffer[index] = terminal_buffer[index+VGA_WIDTH];
	    if (y == VGA_HEIGHT-1) {
		terminal_buffer[index] = vga_entry(' ', terminal_colour);
	    }
	}
    }
}

void terminal_put_char(char c) 
{
    /*
     * Handle new-line chars by moving
     * to a new row
     */
    if (c == '\n') {
	// Skip to end of line
	terminal_column = VGA_WIDTH-1;
    }
    else {
	terminal_put_entry_at(c, terminal_colour,
			  terminal_column, terminal_row);
    }
   
    /*
     * Handle overruns
     */
    terminal_column++;
    if (terminal_column == VGA_WIDTH) {
	terminal_column = 0;
	terminal_row++;
	if (terminal_row == VGA_HEIGHT) {
	    terminal_row--;
	    // Cursor has reached our limit;
	    // we need to scroll up.
	    terminal_scroll_up();
	}
    }
}
 
void terminal_write(const char* data, size_t size) 
{
    for (size_t i = 0; i < size; i++)
	terminal_put_char(data[i]);
}
 
void terminal_write_string(const char* data) 
{
    terminal_write(data, strlen(data));
}
 
void kernel_main(void) 
{
    /* Initialize terminal interface */
    terminal_initialize();
 
    /* Newline support is left as an exercise. */
    terminal_write_string("#################################\n");
    terminal_write_string("#####  Welcome to Henry OS! #####\n");
    terminal_write_string("#################################\n\n");
    terminal_write_string(">> ");
}
