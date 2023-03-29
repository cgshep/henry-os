#ifndef _CONSOLE_H
#define _CONSOLE_H

void console_init();
void console_delete_last_cmd_char();
void console_new_cmd_char(char kbd_char);
void console_process_cmd_buffer();

int hello(char **args, int n_args);
int cmds(char **args, int n_args);

#endif
