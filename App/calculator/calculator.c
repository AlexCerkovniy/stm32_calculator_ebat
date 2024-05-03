#include "calculator.h"
#include "7segment.h"
#include "keyboard.h"

calc_number_t display, result;
float display_float = 0;
float buffer_float = 0;
float result_float = 0;
calc_operation_t operation = CALC_OP_NONE;
bool wait_second_arg = false;
bool update = false;

uint16_t error_timer = 0;
uint8_t error_msg[] = {0x79, 0x50, 0x50};

static void number_set_zero(calc_number_t *number);
static void number_show(calc_number_t *number);
static float number_convert_to_float(calc_number_t *number);
static void number_convert_from_float(float f, calc_number_t *number);

void calculator_init(void){
	number_set_zero(&display);
	result_float = 0;
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

		if(error_timer){
			seven_segment_enable(2, true);
			seven_segment_set_symbol(2, error_msg[0]);
			seven_segment_enable(1, true);
			seven_segment_set_symbol(1, error_msg[1]);
			seven_segment_enable(0, true);
			seven_segment_set_symbol(0, error_msg[2]);

			/* Clear result & display */
			result_float = 0;
			buffer_float = 0;
			wait_second_arg = false;
			number_set_zero(&display);
			operation = CALC_OP_NONE;
			return;
		}

		/* Display data on screen */
		number_show(&display);
	}
}

void calculator_tick(uint16_t period){
	if(error_timer){
		if(error_timer > period){
			error_timer -= period;
		}
		else{
			error_timer = 0;
			update = true;
		}
	}
}

void keyboard_callback(keyboard_key_id key, keyboard_event_id event){
	if(event == KEY_SHORT_PRESS){
		switch(key){
			case KEY_DOT_ID:
				if(display.fraction_digits == 0){
					display.fraction_digits = 1;
				}
				break;

			case KEY_EQUAL_ID:
				if(operation == CALC_OP_NONE){
					return;
				}

				display_float = number_convert_to_float(&display);

				switch(operation){
					case CALC_OP_ADD: buffer_float += display_float; break;
					case CALC_OP_SUBSTRACT: buffer_float -= display_float; break;
					case CALC_OP_MULTIPLY: buffer_float *= display_float; break;
					case CALC_OP_DIVIDE:
						if(display_float == 0){
							error_timer = ERROR_MSG_SHOW_TIME_MS;
							break;
						}

						buffer_float /= display_float;
						break;

					default:
						return;
				}

				wait_second_arg = false;
				result_float = buffer_float;
				number_convert_from_float(result_float, &display);
				break;

			case KEY_ADD_ID:
				operation = CALC_OP_ADD;
				break;

			case KEY_SUBSTRACT_ID:
				operation = CALC_OP_SUBSTRACT;
				break;

			case KEY_MULTIPLY_ID:
				operation = CALC_OP_MULTIPLY;
				break;

			case KEY_DIVIDE_ID:
				operation = CALC_OP_DIVIDE;
				break;

			default:
				if(operation != CALC_OP_NONE && wait_second_arg == false){
					/* Save result on screen to buffer */
					buffer_float = number_convert_to_float(&display);
					number_set_zero(&display);
					wait_second_arg = true;
				}

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
				break;
		}
	}
	else if(event == KEY_LONG_PRESS){
		switch(key){
			case KEY_EQUAL_ID:
				/* Clear result & display */
				result_float = 0;
				buffer_float = 0;
				wait_second_arg = false;
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

static float number_convert_to_float(calc_number_t *number){
	float tmp = 0;

	/* Fraction part */
	if(number->fraction_digits){
		uint32_t i = number->fraction_digits;
		tmp = 1;
		while(i){
			tmp *= 10;
			i--;
		}

		tmp = (float)number->fraction / tmp;
	}

	return (float)number->absolute + tmp;
}

static void number_convert_from_float(float f, calc_number_t *number){
	uint32_t tmp;
	float ftemp;

	number_set_zero(number);

	/* Set sign */
	if(f < 0){
		number->negative = true;
	}

	/* Check range for 8-digit display */
	if(f > 99999999){
		number->absolute = 99999999;
		return;
	}
	else if(f < -9999999){
		number->absolute = 9999999;
		return;
	}

	/* Invert number, for negative sign clear */
	if(number->negative){
		f = -f;
	}

	/* Convert absolute part */
	number->absolute = (uint32_t)f;
	tmp = number->absolute;
	if(tmp){
		while(tmp){
			number->absolute_digits++;
			tmp /= 10;
		}

		number->absolute_digits -= 1;
	}

	/* Convert fraction part */
	tmp = DIGITS_COUNT - number->absolute_digits;
	f = f - (float)number->absolute;

	if(tmp && (f > 0)){
		ftemp = 1;
		while(tmp){
			number->fraction_digits++;
			ftemp *= 10;
			tmp--;
		}

		number->fraction = f * ftemp;

		/* Remove trailing zeros */
		tmp = number->fraction;
		while(tmp % 10 == 0){
			tmp /= 10;
			number->fraction_digits--;
		}

		number->fraction = tmp;
	}
}

