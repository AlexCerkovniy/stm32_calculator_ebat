#include "calculator.h"
#include "7segment.h"
#include "keyboard.h"

float result = 0;
float number = 0;
calc_operation_t operation = CALC_OP_NONE;

void calculator_init(void){
	result = 0;
	number = 0;
	operation = CALC_OP_NONE;
}

void calculator_main(void){

}

void keyboard_callback(keyboard_key_id key, keyboard_event_id event){
	if(event == KEY_SHORT_PRESS){
		switch(key){
			case KEY_DOT_ID:
				break;
			case KEY_EQUAL_ID:
				break;
			case KEY_ADD_ID:
				break;
			case KEY_SUBSTRACT_ID:
				break;
			case KEY_MULTIPLY_ID:
				break;
			case KEY_DIVIDE_ID:
				break;

			default:
				return;
		}
	}
	else if(event == KEY_LONG_PRESS){
		switch(key){
			case KEY_EQUAL_ID:
				/* Clear result & display */
				result = 0;
				number = 0;
				operation = CALC_OP_NONE;
				break;

			case KEY_SUBSTRACT_ID:
				number =-number;
				break;

			default:
				return;
		}
	}
}
