#ifndef CONSOLE_H
#define CONSOLE_H

void console_init();
void console_delete_last_cmd_char();
void console_new_cmd_char(char kbd_char);
void console_process_cmd_buffer();
void tokenise_string(char* str, char* delims);

#endif
