#include "keyboard.h"
#include "tty.h"
#include "string.h"
#include "console.h"

#include <stdint.h>

static uint8_t shift_down = 0;

void keyboard_init()
{
    // Placeholder
}

void print_key(uint8_t ascii_char)
{
    putch(ascii_char);
}

void handle_shift_release(uint8_t masked_scan_code)
{
    if (masked_scan_code == KBD_RLEFT_SHIFT ||
	masked_scan_code == KBD_RRIGHT_SHIFT) {
	shift_down = 0;
    }
}

void handle_shift_down(uint8_t scan_code)
{
    if (scan_code == KBD_RLEFT_SHIFT ||
	scan_code == KBD_RRIGHT_SHIFT) {
	shift_down = 1;
    }
}

uint8_t is_kbd_char_allowed(uint8_t kbd_char)
{
    size_t n = sizeof(allowed) / sizeof(uint8_t);

    for (size_t i = 0; i < n; i++)
	if (kbd_char == allowed[i])
	    return 1;

    return 0;
}

char get_key_char(uint8_t scan_code)
{
    uint8_t kbd_char, masked_scan_code;
    /*
     * Each keyboard interrupt will generate
     * two firings: one for key down and one
     * for the key up event.
     */
    if (scan_code & 0x80) {
	/*
	 * Key up event
	 */
	// Check only the bottom 7 bits
	masked_scan_code = scan_code & 0x7F;
	handle_shift_release(masked_scan_code);
    }
    else {
	/*
	 * Key down event
	 */
	kbd_char = us_ascii_non_shift_codes[scan_code];
	handle_shift_down(scan_code);
	if (shift_down) {
	    kbd_char = us_ascii_shift_codes[scan_code];
	}

	/*
	 * Return only whitelisted chars for now
	 */
	if (is_kbd_char_allowed(kbd_char)) {
	    return kbd_char;
	}
    }

    return KBD_NULL;
}

void process_key(uint8_t scan_code)
{
    /*
     * 1. Get kbd char from scan code.
     * 2. Print the scan code to screen, unless
     *    it's a backspace or enter, and add
     *    it to our buffer.
     * 3. If it's a backspace, delete the previous
     * character.
     */
    char kbd_char = get_key_char(scan_code);
    if (kbd_char != KBD_NULL) {
	if(kbd_char == KBD_BACKSPACE) {
	    console_delete_last_cmd_char();
	}
	else if(kbd_char == KBD_ENTER ||
		kbd_char == KBD_RETURN) {
	    console_process_cmd_buffer();
	}
	else {
	    console_new_cmd_char(kbd_char);
	}
    }
}
