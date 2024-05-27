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
	KEY_0_LED_MASK 			= (1 << 0),
	KEY_1_LED_MASK 			= (1 << 1),
	KEY_2_LED_MASK 			= (1 << 5),
	KEY_3_LED_MASK 			= (1 << 9),
	KEY_4_LED_MASK 			= (1 << 2),
	KEY_5_LED_MASK 			= (1 << 6),
	KEY_6_LED_MASK 			= (1 << 10),
	KEY_7_LED_MASK 			= (1 << 3),
	KEY_8_LED_MASK 			= (1 << 7),
	KEY_9_LED_MASK 			= (1 << 11),
	KEY_DOT_LED_MASK 		= (1 << 4),
	KEY_EQUAL_LED_MASK 		= (1 << 8),
	KEY_ADD_LED_MASK 		= (1 << 12),
	KEY_SUBSTRACT_LED_MASK 	= (1 << 13),
	KEY_MULTIPLY_LED_MASK 	= (1 << 14),
	KEY_DIVIDE_LED_MASK 	= (1 << 15),

	KEY_LED_ALL_MASK		= 0xFFFF
} keyboard_led_mask;

typedef enum {
	KEY_PRESSED =           (1 << 0),
	KEY_RELEASED =          (1 << 1),
    KEY_SHORT_PRESS =       (1 << 2),
	KEY_LONG_PRESS =        (1 << 3)
} keyboard_event_id;

void keyboard_init(void);
void keyboard_main(void);
void keyboard_tick(uint16_t period);
void keyboard_set_led(uint32_t mask);
void keyboard_clear_led(uint32_t mask);
void keyboard_callback(keyboard_key_id key, keyboard_event_id event);

