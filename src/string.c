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
 * Thanks to the OSDev community.
 * src: https://wiki.osdev.org/Printing_To_Screen
 */
char* itoa(int value, char* str, int base)
{
    char * rc;
    char * ptr;
    char * low;
    // Check for supported base.
    if ( base < 2 || base > 36 )
    {
        *str = '\0';
        return str;
    }
    rc = ptr = str;
    // Set '-' for negative decimals.
    if ( value < 0 && base == 10 )
    {
        *ptr++ = '-';
    }
    // Remember where the numbers start.
    low = ptr;
    // The actual conversion.
    do
    {
        // Modulo is negative for negative value. This trick makes abs() unnecessary.
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while ( value );
    // Terminating the string.
    *ptr-- = '\0';
    // Invert the numbers.
    while ( low < ptr )
    {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
    return rc;
}

void* memset(void* bufptr, int value, size_t size) {
    uint8_t* buf = (uint8_t*) bufptr;
    for (size_t i = 0; i < size; i++) {
	buf[i] = (uint8_t) value;
    }
    return bufptr;
}

size_t strlen(const char* str)
{
    size_t i = 0;
    while (str[i]) {
	i++;
    }
    return i;
}

void subchar(char *str, char target_char, char replacement_char)
{
    while(*str) {
	if (*str == target_char)
	    *str = replacement_char;
	str++;
    }
}

uint8_t strcmp(char *str1, char *str2)
{
    while(*str1 && *str1 == *str2) {
	str1++;
	str2++;
    }

    return (uint8_t)*str1 - (uint8_t)*str2;
}
