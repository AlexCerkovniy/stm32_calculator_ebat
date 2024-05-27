#include "keyboard.h"
#include "button.h"
#include "main.h"

static struct{
	button_t button[KEY_COUNT];
	uint32_t led_states;
}keyboard;

static uint8_t column_index = 0;
static uint16_t column_pin[] = {KEY_C1_Pin, KEY_C2_Pin, KEY_C3_Pin, KEY_C4_Pin};

/* Exported buttons state from buttons driver */
extern uint16_t buttons_state;

static void button_callback(uint8_t button_id, button_callback_event_t event);
static void scan_buttons(void);
static void led_show(void);

void keyboard_init(void){
	for(uint32_t btn = 0; btn < KEY_COUNT; btn++){
		BTN_Init(&keyboard.button[btn], KEY_0_ID + btn, 50, 0 /* Disabled */);
		BTN_RegisterCallback(&keyboard.button[btn], button_callback);
	}

	/* Set long-press time for * and OK buttons */
	keyboard.button[KEY_EQUAL_ID].long_press_ms = 500;
	keyboard.button[KEY_SUBSTRACT_ID].long_press_ms = 500;
}

void keyboard_main(void){
	for(uint32_t btn = 0; btn < KEY_COUNT; btn++){
		BTN_Main(&keyboard.button[btn]);
	}
}

void keyboard_tick(uint16_t period){
	/* Scan keyboard buttons */
	scan_buttons();

	/* Update timings for buttons library */
	for(uint32_t btn = 0; btn < KEY_COUNT; btn++){
		BTN_Tick(&keyboard.button[btn], period);
	}
}

void keyboard_set_led(uint32_t mask){
	keyboard.led_states |= mask;
}

void keyboard_clear_led(uint32_t mask){
	keyboard.led_states &= ~mask;
}

static void button_callback(uint8_t button_id, button_callback_event_t event){
	keyboard_event_id kbd_event = KEY_RELEASED;

	switch(event){
		case BTN_STATE_UPDATED:
			if(keyboard.button[button_id].pressed){
				kbd_event = KEY_PRESSED;
			}
			break;

		case BTN_SHORT_PRESS: kbd_event = KEY_SHORT_PRESS; break;
		case BTN_LONG_PRESS: kbd_event = KEY_LONG_PRESS; break;

		default:
			break;
	}

	keyboard_callback(button_id, kbd_event);
}

/* Weak functions for user realization if needed */
__attribute__((weak)) void keyboard_callback(keyboard_key_id key, keyboard_event_id event){}

static void scan_buttons(void){
	uint16_t tmp = 0;

	/* Read coulumn buttons */
	tmp |= (KEY_R1_GPIO_Port->IDR & KEY_R1_Pin)?(0):(1);
	tmp <<= 1;
	tmp |= (KEY_R2_GPIO_Port->IDR & KEY_R2_Pin)?(0):(1);
	tmp <<= 1;
	tmp |= (KEY_R3_GPIO_Port->IDR & KEY_R3_Pin)?(0):(1);
	tmp <<= 1;
	tmp |= (KEY_R4_GPIO_Port->IDR & KEY_R4_Pin)?(0):(1);

	/* Clear last buttons states */
	buttons_state &= ~(0x0F << (column_index * 4));

	/* Set new buttons states */
	buttons_state |= tmp << (column_index * 4);

	/* Select column row */
	HAL_GPIO_WritePin(GPIOB, column_pin[column_index], SET);
	column_index++;
	if(column_index >= 4){
		column_index = 0;
	}

	/* Show next buttons LED's column */
	led_show();

	HAL_GPIO_WritePin(GPIOB, column_pin[column_index], RESET);
}

static void led_show(void){
	/* Clear led rows output */
	GPIOA->ODR &= ~(KEY_LED_R4_Pin|KEY_LED_R3_Pin|KEY_LED_R2_Pin|KEY_LED_R1_Pin);

	/* Set new led rows output */
	uint32_t tmp = (keyboard.led_states >> (column_index * 4)) & 0x0F;
	GPIOA->ODR |= (tmp << 8);
}
