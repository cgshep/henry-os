#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include <stdint.h>

size_t strlen(const char* str);
char* itoa(int val, char* str, int base);
void* memset(void* bufptr, int value, size_t size);
void puts(char* str);
void putch(char c);
void subchar(char *str, char target_char, char replacement_char);
char* strtok(char *s, const char *delim);
char* strtok_r(char *s, const char *delim, char **last);
uint8_t strcmp(char *str1, char *str2);
double atof(const char *str);
char *double_to_string(double value);
int isdigit(int c);
int isspace(int c);

#endif
