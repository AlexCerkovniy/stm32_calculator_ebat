#include "calculator.h"
#include "7segment.h"
#include "keyboard.h"

static uint32_t leds_effects_startup[] = {
		KEY_0_LED_MASK | KEY_DOT_LED_MASK | KEY_EQUAL_LED_MASK | KEY_ADD_LED_MASK,
		KEY_1_LED_MASK | KEY_2_LED_MASK | KEY_3_LED_MASK | KEY_SUBSTRACT_LED_MASK,
		KEY_4_LED_MASK | KEY_5_LED_MASK | KEY_6_LED_MASK | KEY_MULTIPLY_LED_MASK,
		KEY_7_LED_MASK | KEY_8_LED_MASK | KEY_9_LED_MASK | KEY_DIVIDE_LED_MASK,

		KEY_0_LED_MASK | KEY_1_LED_MASK | KEY_4_LED_MASK | KEY_7_LED_MASK,
		KEY_DOT_LED_MASK | KEY_2_LED_MASK | KEY_5_LED_MASK | KEY_8_LED_MASK,
		KEY_EQUAL_LED_MASK | KEY_3_LED_MASK | KEY_6_LED_MASK | KEY_9_LED_MASK,
		KEY_ADD_LED_MASK | KEY_SUBSTRACT_LED_MASK | KEY_MULTIPLY_LED_MASK | KEY_DIVIDE_LED_MASK,

		KEY_DIVIDE_LED_MASK,
		KEY_9_LED_MASK | KEY_MULTIPLY_LED_MASK,
		KEY_8_LED_MASK | KEY_6_LED_MASK | KEY_SUBSTRACT_LED_MASK,
		KEY_7_LED_MASK | KEY_5_LED_MASK | KEY_3_LED_MASK | KEY_ADD_LED_MASK,
		KEY_4_LED_MASK | KEY_2_LED_MASK | KEY_EQUAL_LED_MASK,
		KEY_1_LED_MASK | KEY_DOT_LED_MASK,
		KEY_0_LED_MASK,

		KEY_7_LED_MASK,
		KEY_4_LED_MASK | KEY_8_LED_MASK,
		KEY_1_LED_MASK | KEY_5_LED_MASK | KEY_9_LED_MASK,
		KEY_0_LED_MASK | KEY_2_LED_MASK | KEY_6_LED_MASK | KEY_DIVIDE_LED_MASK,
		KEY_DOT_LED_MASK | KEY_3_LED_MASK | KEY_MULTIPLY_LED_MASK,
		KEY_EQUAL_LED_MASK | KEY_SUBSTRACT_LED_MASK,
		KEY_ADD_LED_MASK,

		KEY_ADD_LED_MASK | KEY_SUBSTRACT_LED_MASK | KEY_MULTIPLY_LED_MASK | KEY_DIVIDE_LED_MASK,
		KEY_EQUAL_LED_MASK | KEY_3_LED_MASK | KEY_6_LED_MASK | KEY_9_LED_MASK,
		KEY_DOT_LED_MASK | KEY_2_LED_MASK | KEY_5_LED_MASK | KEY_8_LED_MASK,
		KEY_0_LED_MASK | KEY_1_LED_MASK | KEY_4_LED_MASK | KEY_7_LED_MASK,

		KEY_7_LED_MASK | KEY_8_LED_MASK | KEY_9_LED_MASK | KEY_DIVIDE_LED_MASK,
		KEY_4_LED_MASK | KEY_5_LED_MASK | KEY_6_LED_MASK | KEY_MULTIPLY_LED_MASK,
		KEY_1_LED_MASK | KEY_2_LED_MASK | KEY_3_LED_MASK | KEY_SUBSTRACT_LED_MASK,
		KEY_0_LED_MASK | KEY_DOT_LED_MASK | KEY_EQUAL_LED_MASK | KEY_ADD_LED_MASK
};

static uint32_t leds_effects_clear[] = {
		KEY_0_LED_MASK | KEY_DOT_LED_MASK | KEY_EQUAL_LED_MASK | KEY_ADD_LED_MASK,
		KEY_1_LED_MASK | KEY_2_LED_MASK | KEY_3_LED_MASK | KEY_SUBSTRACT_LED_MASK,
		KEY_4_LED_MASK | KEY_5_LED_MASK | KEY_6_LED_MASK | KEY_MULTIPLY_LED_MASK,
		KEY_7_LED_MASK | KEY_8_LED_MASK | KEY_9_LED_MASK | KEY_DIVIDE_LED_MASK
};

calc_number_t display, result;
float result_float = 0;
calc_operation_t operation = CALC_OP_NONE;
bool next_argument = false;
bool update = false;

uint16_t error_timer = 0;
uint8_t error_msg[] = {0x79, 0x50, 0x50};

static void calculator_calc(void);

static void number_set_zero(calc_number_t *number);
static void number_show(calc_number_t *number);
static float number_convert_to_float(calc_number_t *number);
static void number_convert_from_float(float f, calc_number_t *number);

static void keyboard_run_led_effect(uint32_t *effect, uint32_t steps, uint32_t speed){
		for(uint8_t i = 0; i < steps; i++){
			keyboard_clear_led(KEY_LED_ALL_MASK);
			keyboard_set_led(effect[i]);
			HAL_Delay(speed);
		}
		keyboard_clear_led(KEY_LED_ALL_MASK);
}

void calculator_greetings(void){
	/* Disable all segments */
	for(uint8_t seg = 0; seg < DIGITS_COUNT; seg++){
		seven_segment_enable(seg, false);
		seven_segment_set_dot(seg, false);
	}

	/* Set some text: CALC */
	seven_segment_enable(7, true);
	seven_segment_set_symbol(7, 0x39);
	seven_segment_enable(6, true);
	seven_segment_set_symbol(6, 0x77);
	seven_segment_enable(5, true);
	seven_segment_set_symbol(5, 0x38);
	seven_segment_enable(4, true);
	seven_segment_set_symbol(4, 0x39);

	HAL_Delay(1000);

	/* Disable all segments */
	for(uint8_t seg = 0; seg < DIGITS_COUNT; seg++){
		seven_segment_enable(seg, false);
		seven_segment_set_dot(seg, false);
	}

	/* Set some text: EL21 */
	seven_segment_enable(4, true);
	seven_segment_set_symbol(4, 0x79);
	seven_segment_enable(3, true);
	seven_segment_set_symbol(3, 0x38);
	seven_segment_enable(2, true);
	seven_segment_set_symbol(2, 0x40);
	seven_segment_enable(1, true);
	seven_segment_set_number(1, 2);
	seven_segment_enable(0, true);
	seven_segment_set_number(0, 1);

	HAL_Delay(1000);
}

void calculator_init(void){
	calculator_greetings();
	keyboard_run_led_effect(leds_effects_startup, sizeof(leds_effects_startup)/sizeof(leds_effects_startup[0]), 75);

	/* Disable all segments */
	for(uint8_t seg = 0; seg < DIGITS_COUNT; seg++){
		seven_segment_enable(seg, false);
		seven_segment_set_dot(seg, false);
	}

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
			next_argument = false;
			number_set_zero(&display);
			operation = CALC_OP_NONE;
			keyboard_clear_led(KEY_ADD_LED_MASK|KEY_SUBSTRACT_LED_MASK|KEY_MULTIPLY_LED_MASK|KEY_DIVIDE_LED_MASK);
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
	uint8_t digits_limit;

	if(event == KEY_SHORT_PRESS){
		switch(key){
			case KEY_DOT_ID:
				if(!next_argument && display.fraction_digits == 0){
					display.fraction_digits = 1;
				}
				break;

			case KEY_EQUAL_ID:
				calculator_calc();
				operation = CALC_OP_NONE;
				keyboard_clear_led(KEY_ADD_LED_MASK|KEY_SUBSTRACT_LED_MASK|KEY_MULTIPLY_LED_MASK|KEY_DIVIDE_LED_MASK);
				break;

			case KEY_ADD_ID:
				calculator_calc();
				operation = CALC_OP_ADD;
				keyboard_clear_led(KEY_ADD_LED_MASK|KEY_SUBSTRACT_LED_MASK|KEY_MULTIPLY_LED_MASK|KEY_DIVIDE_LED_MASK);
				keyboard_set_led(KEY_ADD_LED_MASK);
				break;

			case KEY_SUBSTRACT_ID:
				calculator_calc();
				operation = CALC_OP_SUBSTRACT;
				keyboard_clear_led(KEY_ADD_LED_MASK|KEY_SUBSTRACT_LED_MASK|KEY_MULTIPLY_LED_MASK|KEY_DIVIDE_LED_MASK);
				keyboard_set_led(KEY_SUBSTRACT_LED_MASK);
				break;

			case KEY_MULTIPLY_ID:
				calculator_calc();
				operation = CALC_OP_MULTIPLY;
				keyboard_clear_led(KEY_ADD_LED_MASK|KEY_SUBSTRACT_LED_MASK|KEY_MULTIPLY_LED_MASK|KEY_DIVIDE_LED_MASK);
				keyboard_set_led(KEY_MULTIPLY_LED_MASK);
				break;

			case KEY_DIVIDE_ID:
				calculator_calc();
				operation = CALC_OP_DIVIDE;
				keyboard_clear_led(KEY_ADD_LED_MASK|KEY_SUBSTRACT_LED_MASK|KEY_MULTIPLY_LED_MASK|KEY_DIVIDE_LED_MASK);
				keyboard_set_led(KEY_DIVIDE_LED_MASK);
				break;

			default:
				if(next_argument){
					next_argument = false;
					number_set_zero(&display);
				}

				digits_limit = DIGITS_COUNT;
				if(display.negative){
					digits_limit -= 1;
				}

				if(display.fraction_digits){
					if(display.fraction){
						if(display.fraction_digits < digits_limit){
							display.fraction *= 10;
							display.fraction += key;
							display.fraction_digits++;
						}
					}
					else{
						display.fraction += key;
					}
				}
				else if(display.absolute_digits < digits_limit){
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
				next_argument = false;
				number_set_zero(&display);
				keyboard_clear_led(KEY_ADD_LED_MASK|KEY_SUBSTRACT_LED_MASK|KEY_MULTIPLY_LED_MASK|KEY_DIVIDE_LED_MASK);
				keyboard_run_led_effect(leds_effects_clear, sizeof(leds_effects_clear)/sizeof(leds_effects_clear[0]), 30);
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

static void calculator_calc(void){
	static float tmp;

	if(operation == CALC_OP_NONE){
		result_float = number_convert_to_float(&display);
		next_argument = true;
		return;
	}

	tmp = number_convert_to_float(&display);

	switch(operation){
		case CALC_OP_ADD: result_float += tmp; break;
		case CALC_OP_SUBSTRACT: result_float -= tmp; break;
		case CALC_OP_MULTIPLY: result_float *= tmp; break;
		case CALC_OP_DIVIDE:
			if(tmp == 0){
				error_timer = ERROR_MSG_SHOW_TIME_MS;
				break;
			}

			result_float /= tmp;
			break;

		default:
			return;
	}

	number_convert_from_float(result_float, &display);
	operation = CALC_OP_NONE;
	next_argument = true;
}

static void number_set_zero(calc_number_t *number){
	number->absolute = 0;
	number->absolute_digits = 1;
	number->fraction = 0;
	number->fraction_digits = 0;
	number->negative = false;
}

static void number_show(calc_number_t *number){
	uint8_t digits_capacity = DIGITS_COUNT;
	uint8_t digits_absolute = number->absolute_digits;
	uint8_t digits_fraction = 0;
	uint8_t digit_offset = 0;
	uint32_t temp;
	uint8_t i;

	/* Calculate digit offset for fraction */
	if(number->fraction_digits){
		if(number->fraction > 0){
			if(number->negative){
				digits_capacity--;
			}

			if(number->absolute_digits >= digits_capacity){
				/* Show only dot, we don't have space for fraction show */
				seven_segment_set_dot(0, true);
			}
			else if(DIGITS_COUNT - number->absolute_digits > number->fraction_digits){
				digits_fraction = number->fraction_digits;
				digit_offset = digits_fraction;
			}
			else{
				digits_fraction = digits_capacity - number->absolute_digits;
				digit_offset = digits_fraction;
			}

			if(number->negative){
				seven_segment_enable(digits_fraction + number->absolute_digits, true);
				seven_segment_set_symbol(digits_fraction + number->absolute_digits, 0x40);
			}
		}
		else{
			/* Show only dot */
			seven_segment_set_dot(0, true);
		}
	}
	else{
		/* If number is negative - check if number digits fit on screen */
		if(number->negative){
			if(digits_absolute >= digits_capacity){
				digits_absolute--;

				if(number->absolute >= 10000000){
					number->absolute = 9999999;
				}
			}

			seven_segment_enable(digits_absolute, true);
			seven_segment_set_symbol(digits_absolute, 0x40);
		}
	}

	/* Show absolute part */
	temp = number->absolute;
	for(i = 0; i < digits_absolute; i++){
		seven_segment_enable(i + digit_offset, true);
		seven_segment_set_number(i + digit_offset, temp % 10);
		temp /= 10;
	}

	/* Show fraction part */
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

	tmp += (float)number->absolute;

	if(number->negative){
		tmp = -tmp;
	}

	return tmp;
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

