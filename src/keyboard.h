#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

void keyboard_init();
void process_key(uint8_t scan_code);
char get_key_char(uint8_t scan_code);
uint8_t is_kbd_char_allowed(uint8_t kbd_char);

#define KBD_NULL 0
#define KBD_ESC    27
#define KBD_BACKSPACE '\b'
#define KBD_TAB       '\t'
#define KBD_ENTER     '\n'
#define KBD_RETURN    '\r'
#define KBD_NEWLINE   ENTER

//#define    KBD_ESC         1
#define    KBD_F1          0x80
#define    KBD_F2         (KBD_F1 + 1)
#define    KBD_F3         (KBD_F2 + 1)
#define    KBD_F4         (KBD_F3 + 1)
#define    KBD_F5         (KBD_F4 + 1)
#define    KBD_F6         (KBD_F5 + 1)
#define    KBD_F7         (KBD_F6 + 1)
#define    KBD_F8         (KBD_F7 + 1)
#define    KBD_F9         (KBD_F8 + 1)
#define    KBD_F10        (KBD_F9 + 1)
#define    KBD_F11        (KBD_F10 + 1)
#define    KBD_F12        (KBD_F11 + 1)

#define    KBD_INS         0x90
#define    KBD_DEL        (KBD_INS + 1)
#define    KBD_HOME       (KBD_DEL + 1)
#define    KBD_END        (KBD_HOME + 1)
#define    KBD_PGUP       (KBD_END + 1)
#define    KBD_PGDN       (KBD_PGUP + 1)
#define    KBD_LEFT       (KBD_PGDN + 1)
#define    KBD_UP         (KBD_LEFT + 1)
#define    KBD_DOWN       (KBD_UP + 1)
#define    KBD_RIGHT      (KBD_DOWN + 1)

// "Meta" keys
#define    KBD_META_ALT     0x0200                                // Alt is pressed
#define    KBD_META_CTRL    0x0400                                // Ctrl is pressed
#define    KBD_META_SHIFT   0x0800                                // Shift is pressed
#define    KBD_META_ANY    (KBD_META_ALT | KBD_META_CTRL | KBD_META_SHIFT)
#define    KBD_META_CAPS    0x1000                                // CapsLock is on
#define    KBD_META_NUM     0x2000                                // NumLock is on
#define    KBD_META_SCRL    0x4000                                // ScrollLock is on

// Other keys
#define    KBD_PRNT    ( KBD_RETURN + 1 )
#define    KBD_PAUSE   ( KBD_PRNT + 1 )
#define    KBD_LWIN    ( KBD_PAUSE + 1 )
#define    KBD_RWIN    ( KBD_LWIN + 1 )
#define    KBD_MENU    ( KBD_RWIN + 1 )

#define    KBD_RLEFT_CTRL        0x1D
#define    KBD_RRIGHT_CTRL       0x1D

#define    KBD_RLEFT_ALT         0x38
#define    KBD_RRIGHT_ALT        0x38

#define    KBD_RLEFT_SHIFT       0x2A
#define    KBD_RRIGHT_SHIFT      0x36

#define    KBD_RCAPS_LOCK        0x3A
#define    KBD_RSCROLL_LOCK      0x46
#define    KBD_RNUM_LOCK         0x45
#define    KBD_RDEL              0x53

#define    MAX_KEYBOARD_BUFFER   256

/*
 * Scan code look up tables for non-shift and shifted
 * keys respectively.
 */
static const uint8_t us_ascii_non_shift_codes[] = {
    KBD_NULL, KBD_ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', KBD_BACKSPACE,
    KBD_TAB, 'q', 'w',   'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',   '[', ']', KBD_ENTER, 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, 0, 0, ' ', 0,
    KBD_F1, KBD_F2, KBD_F3, KBD_F4, KBD_F5, KBD_F6, KBD_F7, KBD_F8, KBD_F9, KBD_F10, 0, 0,
    KBD_HOME, KBD_UP, KBD_PGUP,'-', KBD_LEFT, '5', KBD_RIGHT, '+', KBD_END, KBD_DOWN, KBD_PGDN, KBD_INS, KBD_DEL, 0, 0, 0, KBD_F11, KBD_F12
};

static const uint8_t us_ascii_shift_codes[] = {
    KBD_NULL, KBD_ESC, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', KBD_BACKSPACE,
    KBD_TAB, 'Q', 'W',   'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',   '{', '}', KBD_ENTER, 0,
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~', 0, '|',
    'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, 0, 0, ' ', 0,
    KBD_F1, KBD_F2, KBD_F3, KBD_F4, KBD_F5, KBD_F6, KBD_F7, KBD_F8, KBD_F9, KBD_F10, 0, 0,
KBD_HOME, KBD_UP, KBD_PGUP, '-', KBD_LEFT, '5',   KBD_RIGHT, '+', KBD_END, KBD_DOWN, KBD_PGDN, KBD_INS, KBD_DEL, 0, 0, 0, KBD_F11, KBD_F12
};

static const uint8_t allowed[] = {
     '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', KBD_BACKSPACE,
     'Q', 'W',   'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', KBD_ENTER,
     'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~', '|',
     'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 
     '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
     'q', 'w',   'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', ' ', '+'
};

#endif
