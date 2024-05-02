#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef enum {
	KEY_0_ID = 0,
	KEY_1_ID,
	KEY_2_ID,
	KEY_3_ID,
	KEY_4_ID,
	KEY_5_ID,
	KEY_6_ID,
	KEY_7_ID,
	KEY_8_ID,
	KEY_9_ID,
	KEY_DOT_ID,
	KEY_EQUAL_ID,
	KEY_ADD_ID,
	KEY_SUBSTRACT_ID,
	KEY_MULTIPLY_ID,
	KEY_DIVIDE_ID,

	KEY_COUNT
} keyboard_key_id;

typedef enum {
	KEY_PRESSED =           (1 << 0),
	KEY_RELEASED =          (1 << 1),
    KEY_SHORT_PRESS =       (1 << 2),
	KEY_LONG_PRESS =        (1 << 3)
} keyboard_event_id;

void keyboard_init(void);
void keyboard_main(void);
void keyboard_tick(uint16_t period);
void keyboard_callback(keyboard_key_id key, keyboard_event_id event);

