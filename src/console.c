#include "tty.h"
#include "string.h"
#include "console.h"

#define MAX_CMD_BUFFER 255
#define DELIM_SPACE ' '

/*
 * = {0} isn't actually needed because global
 * arrays are zero-initialised. But it's here
 * in case we forget later.
 */
char cmd_buffer[MAX_CMD_BUFFER] = {0};
uint8_t cmd_buffer_pos;

void console_init()
{
    cmd_buffer_pos = cmd_buffer[0];
}

void console_delete_last_cmd_char()
{
    terminal_cmd_delete_last_char();
    cmd_buffer[cmd_buffer_pos--] = 0;
}

void console_new_cmd_char(char kbd_char)
{
    if (cmd_buffer_pos < MAX_CMD_BUFFER) {
	cmd_buffer[cmd_buffer_pos++] = kbd_char;
	terminal_cmd_putch(kbd_char);
    }
}

void tokenise_string(char *str, char *delims)
{
    char *tok_ptr;
    puts("Printing tokens...\n");
    tok_ptr = strtok(str, delims);
    while(tok_ptr) {
	puts(tok_ptr);
	puts(" # ");
	tok_ptr = strtok(NULL, delims);
    }
}

void reset_cmd_buffer()
{
    memset(cmd_buffer, 0, sizeof(char)*MAX_CMD_BUFFER);
    cmd_buffer_pos = cmd_buffer[0];
}

void console_process_cmd_buffer()
{
    /*
     * 1. Tokenise the command buffer using TOKEN_SPACE.
     * 2. Reset the commnad buffer.
     */
    tokenise_string(cmd_buffer, " ");
    
    // Reset
    reset_cmd_buffer();
    terminal_new_cmd();
}
