#include "tty.h" 

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
 
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_colour;
uint16_t* terminal_buffer;


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
 
