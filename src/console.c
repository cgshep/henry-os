#include "tty.h"
#include "string.h"
#include "console.h"

#define MAX_CMD_BUFFER 32
#define MAX_ARGS       64
#define CONSOLE_DELIMS    " "

static char cmd_buffer[MAX_CMD_BUFFER];
static size_t cmd_buffer_pos;

typedef int (*func_ptr_t)(char **args, int n_args);

typedef struct {
    char *name;
    func_ptr_t func;
} cmd_name_func_tuple_t;

static int hello(char **args, int n_args)
{
    puts("\nMy hello world!\n");
    return 0;
}

static int calculator(char **args, int n_args)
{
    puts("\nMy calculator!\n");
    return 0;
}

static const cmd_name_func_tuple_t cmds[] = {
    {"calc", calculator},
    {"hello", hello},
};

static int search_cmd_idx(char *cmd_name)
{
    size_t i = -1;

    for(i=0; i<sizeof(cmds)/sizeof(cmds[0]); i++)
	if(strcmp(cmds[i].name, cmd_name) == 0)
	    return i;

    return -1;
}

static int tokenise_string(char *str, char *delims, char **args)
{
    char *tok_ptr;
    size_t i = 0;

    if (str == NULL || delims == NULL || !strlen(str)) {
	return -1;
    }

    tok_ptr = strtok(str, delims);

    if(tok_ptr == NULL) {
	return -1;
    }

    while(tok_ptr) {
	args[i] = tok_ptr;
	tok_ptr = strtok(NULL, delims);
	i++;
    }

    return i;
}

static int parse_cmd(char **args, int n_args)
{
    if (strcmp(args[0], NULL) == 0) {
	return -1;
    }
    else {
	return search_cmd_idx(args[0]);
    }
}

static void reset_cmd_buffer()
{
    memset(cmd_buffer, 0, sizeof(char)*MAX_CMD_BUFFER);
    cmd_buffer_pos = cmd_buffer[0];
}

void console_init()
{
    cmd_buffer_pos = cmd_buffer[0];
}

void console_delete_last_cmd_char()
{
    if (cmd_buffer_pos > 0) {
	terminal_cmd_delete_last_char();
	cmd_buffer[cmd_buffer_pos--] = 0;
    }
}

void console_new_cmd_char(char kbd_char)
{
    if (cmd_buffer_pos < MAX_CMD_BUFFER) {
	cmd_buffer[cmd_buffer_pos++] = kbd_char;
	terminal_cmd_putch(kbd_char);
    }
}

void console_process_cmd_buffer()
{
    /*
     * 1. Tokenise the command buffer using TOKEN_SPACE.
     * 
     * 2. Reset the commnad buffer.
     */
    char *cmd_args[MAX_ARGS] = {0};
    int ret, cmd_idx, n_args;
    
    n_args = tokenise_string(cmd_buffer,
			     CONSOLE_DELIMS,
			     cmd_args);

    if (n_args > 0) {
	cmd_idx = parse_cmd(cmd_args, n_args);

	if (cmd_idx < 0) {
	    puts("Invalid command.");
	}
	else {
	    ret = cmds[cmd_idx].func(cmd_args, n_args);
	}
    }
    else {
	puts("Invalid syntax.");
    }

    if(ret < 0) {
	puts("Error!");
    }

    reset_cmd_buffer();
    terminal_new_cmd();
}
