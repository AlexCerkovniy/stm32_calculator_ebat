#include "calculator.h"
#include "7segment.h"
#include "keyboard.h"

calc_number_t display;
float result = 0;
calc_operation_t operation = CALC_OP_NONE;
bool show_result = true;
bool update = false;

static void number_set_zero(calc_number_t *number);
static void number_show(calc_number_t *number);

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
			seven_segment_set_dot(seg, false);
		}

		/* Show numbers */
		if(show_result){
			if(result == 0){
				seven_segment_enable(0, true);
				seven_segment_set_number(0, 0);
			}
			else{
				/* Convert float to display number type */
			}
		}
		else{
			number_show(&display);
		}
	}
}

void keyboard_callback(keyboard_key_id key, keyboard_event_id event){
	if(event == KEY_SHORT_PRESS){
		switch(key){
			case KEY_DOT_ID:
				if(show_result){
					show_result = false;
					number_set_zero(&display);
				}

				if(display.fraction_digits == 0){
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
							if(display.fraction_digits < DIGITS_COUNT){
								display.fraction *= 10;
								display.fraction += key;
								display.fraction_digits++;
							}
						}
						else{
							display.fraction += key;
						}
					}
					else if(display.absolute_digits < DIGITS_COUNT){
						if(display.absolute == 0 && key == KEY_0_ID){
							break;
						}

						if(display.absolute){
							display.absolute_digits++;
							display.absolute *= 10;
						}

						display.absolute += key;
					}
				}
				break;
		}
	}
	else if(event == KEY_LONG_PRESS){
		switch(key){
			case KEY_EQUAL_ID:
				/* Clear result & display */
				result = 0;
				show_result = true;
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
	else{
		return;
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

static void number_show(calc_number_t *number){
	uint8_t digits_absolute = number->absolute_digits;
	uint8_t digits_fraction = 0;
	uint8_t digit_offset = 0;
	uint32_t temp;
	uint8_t i;

	/* Calculate digit offset for fraction */
	if(number->fraction_digits){
		if(number->fraction > 0){
			if(number->absolute_digits == DIGITS_COUNT){
				/* Show only dot, we don't have space for fraction show */
				seven_segment_set_dot(0, true);
			}
			else if(DIGITS_COUNT - number->absolute_digits > number->fraction_digits){
				digits_fraction = number->fraction_digits;
				digit_offset = digits_fraction;
			}
			else{
				digits_fraction = DIGITS_COUNT - number->absolute_digits;
				digit_offset = digits_fraction;
			}
		}
		else{
			/* Show only dot */
			seven_segment_set_dot(0, true);
		}
	}

	/* Show absolute part */
	temp = number->absolute;
	for(i = 0; i < digits_absolute; i++){
		seven_segment_enable(i + digit_offset, true);
		seven_segment_set_number(i + digit_offset, temp % 10);
		temp /= 10;
	}

	/* Show absolute part */
	if(digits_fraction){
		temp = number->fraction;
		if(digits_fraction < number->fraction_digits){
			i = number->fraction_digits - digits_fraction;
			while(i){
				temp /= 10;
				i--;
			}
		}

		seven_segment_set_dot(digit_offset, true);
		for(i = 0; i < digits_fraction; i++){
			seven_segment_enable(i, true);
			seven_segment_set_number(i, temp % 10);
			temp /= 10;
		}
	}
}
