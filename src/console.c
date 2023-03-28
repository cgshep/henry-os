#include "tty.h"
#include "string.h"
#include "console.h"
#include "applications.h"

#define MAX_CMD_BUFFER    32
#define MAX_ARGS          16
#define CONSOLE_DELIMS    " "
#define CONSOLE_CMD_ARROW "--> "

typedef int (*cmd_func_ptr_t)(char **args, int n_args);

typedef struct {
    char *name;
    cmd_func_ptr_t func;
} console_cmd_tuple_t;

static const console_cmd_tuple_t console_cmds[] = {
    {"calc", calculator},
    {"hello", hello},
    {"cmds", cmds},
};

static char cmd_buffer[MAX_CMD_BUFFER];
static size_t cmd_buffer_pos;
static size_t n_cmds = sizeof(console_cmds) / sizeof(console_cmds[0]);

/*
 * Console commands
 */
int hello(char **args, int n_args)
{
    puts("Hello, world!");
    return 0;
}

int cmds(char **args, int n_args)
{
    puts("Supported commands:\n");

    for(size_t i = 0; i < n_cmds; i++) {
	puts("--> ");
        puts(console_cmds[i].name);
	if (i < n_cmds-1) {
	    putch('\n');
	}
    }

    return 0;
}

/*
 * Console parser
 */
static int search_cmd_idx(char *cmd_name)
{
    for(size_t i = 0; i < n_cmds; i++)
	if(strcmp(console_cmds[i].name, cmd_name) == 0)
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

void console_process_cmd_buffer()
{
    char *cmd_args[MAX_ARGS] = {0};
    int ret, cmd_idx, n_args;
    
    n_args = tokenise_string(cmd_buffer,
			     CONSOLE_DELIMS,
			     cmd_args);

    if (n_args > 0) {
	putch('\n');
	cmd_idx = parse_cmd(cmd_args, n_args);

	if (cmd_idx < 0) {
	    puts("Invalid command.");
	}
	else {
	    ret = console_cmds[cmd_idx].func(cmd_args, n_args);
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

/*
 * Console initialisation
 */
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
