#include "calculator.h"
#include "7segment.h"
#include "keyboard.h"

calc_number_t display;
float result = 0;
calc_operation_t operation = CALC_OP_NONE;
bool show_result = true;
bool update = false;

static void number_set_zero(calc_number_t *number);

void calculator_init(void){
	number_set_zero(&display);
	result = 0;
	operation = CALC_OP_NONE;
	update = true;
}

void calculator_main(void){
	if(update){
		update = false;

		/* Disable all segments */
		for(uint8_t seg = 0; seg < DIGITS_COUNT; seg++){
			seven_segment_enable(seg, false);
		}

		/* Show numbers */
		if(show_result){
			if(result == 0){
				seven_segment_enable(0, true);
				seven_segment_set_number(0, 0);
			}
		}
	}
}

void keyboard_callback(keyboard_key_id key, keyboard_event_id event){
	if(event == KEY_SHORT_PRESS){
		switch(key){
			case KEY_DOT_ID:
				if(display.fraction_digits){
					display.fraction_digits++;
				}
				else{
					display.fraction_digits = 1;
				}
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
				if(show_result){
					show_result = false;

					/* Set-up display number */
					number_set_zero(&display);
					display.absolute = key;
				}
				else {
					if(display.fraction_digits){
						if(display.fraction){
							display.fraction *= 10;
							display.fraction += key;
						}
						else{
							display.fraction += key;
						}
					}
					else if(display.absolute_digits < DIGITS_COUNT){
						display.absolute *= 10;
						display.absolute += key;
						display.absolute_digits++;
					}
				}
				return;
		}
	}
	else if(event == KEY_LONG_PRESS){
		switch(key){
			case KEY_EQUAL_ID:
				/* Clear result & display */
				result = 0;
				number_set_zero(&display);
				operation = CALC_OP_NONE;
				break;

			case KEY_SUBSTRACT_ID:
				display.negative = true;
				break;

			default:
				return;
		}
	}

	update = true;
}

static void number_set_zero(calc_number_t *number){
	number->absolute = 0;
	number->absolute_digits = 1;
	number->fraction = 0;
	number->fraction_digits = 0;
	number->negative = false;
}
