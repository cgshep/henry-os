#include "tty.h"
#include "string.h"

#include <stdbool.h>
#include <stddef.h>

#define DOUBLE_STR_BUFFER_SIZE 64

int isdigit(int c)
{
    return (c >= '0' && c <= '9');
}

int isspace(int c)
{
    return ((c == '\n') || (c == '\t') || (c == ' '));
}

void putch(char c)
{
    terminal_put_char(c);
}

void puts(char* str)
{
    terminal_write_string(str);
}

void puthex(uint32_t n)
{
    terminal_write_hex(n);
}

void putint(uint32_t n)
{
    terminal_write_int(n);
}

double pow(double base, int exponent)
{
    double result = 1.0;

    // Handle negative exponents
    if (exponent < 0) {
        base = 1.0 / base;
        exponent = -exponent;
    }

    // Calculate the power using repeated multiplication
    for (int i = 0; i < exponent; i++) {
        result *= base;
    }

    return result;
}

double atof(const char *str)
{
    double result = 0.0;
    int sign = 1;
    double fraction = 0.0;
    bool is_decimal = false;
    int decimal_places = 0;

    // Skip whitespace characters
    while (isspace(*str)) {
        str++;
    }

    // Determine sign
    if (*str == '-') {
        sign = -1;
        str++;
    } else if (*str == '+') {
        str++;
    }

    // Process the string
    while (*str != '\0') {
        if (*str == '.' && !is_decimal) {
            is_decimal = true;
            str++;
            continue;
        }

        if (isdigit(*str)) {
            if (is_decimal) {
                fraction = fraction * 10.0 + (*str - '0');
                decimal_places++;
            } else {
                result = result * 10.0 + (*str - '0');
            }
        } else {
            break;
        }
        str++;
    }

    // Combine integer and fractional parts
    result += fraction / pow(10.0, decimal_places);
    result *= sign;

    return result;
}

void reverse(char *str, int len)
{
    int start = 0;
    int end = len - 1;
    char temp;

    while (start < end) {
        temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

char *double_to_string(double value)
{
    static char buffer[DOUBLE_STR_BUFFER_SIZE];
    int int_part = (int)value;
    double fractional_part = value - int_part;
    int i = 0;
    bool is_negative = false;

    if (value < 0) {
        is_negative = true;
        int_part = -int_part;
        fractional_part = -fractional_part;
    }

    do {
        buffer[i++] = int_part % 10 + '0';
        int_part /= 10;
    } while (int_part != 0);

    if (is_negative) {
        buffer[i++] = '-';
    }

    reverse(buffer, i);

    buffer[i++] = '.';

    int num_decimal_digits = 6;
    for (int j = 0; j < num_decimal_digits; j++) {
        fractional_part *= 10;
        int digit = (int)fractional_part;
        buffer[i++] = digit + '0';
        fractional_part -= digit;
    }

    buffer[i] = '\0';
    return buffer;
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

void *memcpy(void *dest, const void *src, size_t n)
{
    char *d = (char *)dest;
    const char *s = (const char *)src;

    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }

    return dest;
}

void *memset(void* bufptr, int value, size_t n)
{
    uint8_t* buf = (uint8_t*) bufptr;

    for (size_t i = 0; i < n; i++) {
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
