#include "keyboard.h"
#include "button.h"

static struct{
	bool state;
	button_t button[KEY_COUNT];
}keyboard;

static void _buttonCallback(uint8_t button_id, button_callback_event_t event);

void keyboard_init(void){

}

void keyboard_main(void){

}

void keyboard_tick(uint16_t period){

}

static void _buttonCallback(uint8_t button_id, button_callback_event_t event){
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
