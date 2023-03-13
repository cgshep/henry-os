#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include <stdint.h>

size_t strlen(const char* str);
char* itoa(int val, int base);
void* memset(void* bufptr, int value, size_t size);
void puts(char *str);
void putch(char c);

#endif
