#include "tty.h"
#include "string.h"

#include <stddef.h>

/*
 * puts is nothing special; just a wrapper
 * for terminal_write_string from tty.h
 */
void puts(char* str)
{
    terminal_write_string(str);
}

void putch(char c)
{
    terminal_put_char(c);
}

/*
 * Thanks to Robert Jan Schaper
https://groups.google.com/g/comp.lang.c++.moderated/c/ihafayWmWU8#00c4c8fcb24dd1cc
 */
char* itoa(int val, int base)
{
	static char buf[32] = {0};
	int i = 30;
	for(; val && i ; --i, val /= base)
	    buf[i] = "0123456789abcdef"[val % base];
	return &buf[i+1];
}

void* memset(void* bufptr, int value, size_t size) {
	unsigned char* buf = (unsigned char*) bufptr;
	for (size_t i = 0; i < size; i++) {
	    buf[i] = (unsigned char) value;
	}
	return bufptr;
}

size_t strlen(const char* str)
{
	size_t len = 0;
	while (str[len])
	    len++;
	return len;
}


void subchar(char *str, char target_char, char replacement_char)
{
    while(*str) {
	if (*str == target_char)
	    *str = replacement_char;
	str++;
    }
}
