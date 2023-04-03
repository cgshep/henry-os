#include "tty.h"
#include "string.h"
#include "pic.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
 
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_colour;
uint16_t* terminal_buffer;
size_t terminal_cmd_start_row;
size_t terminal_cmd_start_col;

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

static const char *banner =
    "\n @@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n"\
    " @                           @\n"\
    " @    /\\___/\\                @\n"\
    " @    | o o |                @\n"\
    " @   __\\_^_/__               @\n" \
    " @  (__/   \\__)   Henry OS   @\n"\
    " @   _|  .  |_               @\n"\
    " @  (__\\___/__)              @\n" \
    " @                           @\n"\
    " @@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";

static void update_cursor(int x, int y)
{
    uint16_t pos = y * VGA_WIDTH + x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t) (pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void terminal_print_banner()
{
    terminal_write_string(banner);
    terminal_new_cmd();
}

void terminal_init() 
{
    terminal_row = 0;
    terminal_column = 0;
    terminal_colour = vga_entry_colour(VGA_COLOUR_WHITE,
				       VGA_COLOUR_BLUE);
    terminal_buffer = (uint16_t*) VGA_MEMORY_LOCATION;
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
    update_cursor(x, y);
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

void terminal_cmd_delete_last_char()
{
    /*
     * Do nothing if the cursor is at the point
     * where the command began.
     */
    if (terminal_column == terminal_cmd_start_col &&
	terminal_row == terminal_cmd_start_row) {
	return;
    }

    /*
     * Handle underruns.
     */
    if (terminal_row > terminal_cmd_start_row &&
	terminal_column == 0) {
	terminal_row--;
	terminal_column = VGA_WIDTH-1;
	terminal_put_entry_at(' ', terminal_colour,
			      terminal_column, terminal_row);
    }
    else {
	terminal_column--;
	terminal_put_entry_at(' ', terminal_colour,
			      terminal_column, terminal_row);
    }
}

void terminal_new_cmd()
{
    terminal_put_char('\n');
    terminal_cmd_start_row = terminal_row;
    terminal_write_string(TERMINAL_PROMPT);
    terminal_cmd_start_col = terminal_column;
}

void terminal_cmd_putch(char c)
{
    terminal_put_char(c);
}

void terminal_put_char(char c) 
{
    /*
     * Handle new-line chars.
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

void terminal_write_int(uint32_t val)
{
    char tmp_str[10]; // 32-bit int = 10 decimal chars
    terminal_write_string(itoa(val, tmp_str, 10));
}

void terminal_write_hex(uint32_t val)
{
    char tmp_str[8];
    terminal_write_string("0x");
    terminal_write_string(itoa(val, tmp_str, 16));
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
