#include "tty.h"
#include "console.h"

#define MAX_CMD_BUFFER 256
uint8_t cmd_buffer[MAX_CMD_BUFFER];
uint8_t cmd_buffer_ptr;

void console_init()
{
    // do we have to zero the buffer
    cmd_buffer_ptr = cmd_buffer[0];
}

void console_delete_last_cmd_char()
{
    terminal_cmd_delete_last_char();
    cmd_buffer[cmd_buffer_ptr--] = 0;
}

void console_new_cmd_char(char kbd_char)
{
    if (cmd_buffer_ptr < MAX_CMD_BUFFER) {
	cmd_buffer[cmd_buffer_ptr++] = kbd_char;
	terminal_cmd_putch(kbd_char);
    }
}

void console_process_cmd_buffer()
{
    terminal_new_cmd();
}
