#include "7segment.h"
#include "main.h"

/* 0 - 9 digits codes */
static const uint8_t digits[] = {63, 6, 91, 79, 102, 109, 125, 7, 127, 111};

struct {
		GPIO_TypeDef *GPIOx;
		uint16_t GPIO_Pin;
} digit_hw[] = {
		{D1_GPIO_Port, D1_Pin},
		{D2_GPIO_Port, D2_Pin},
		{D3_GPIO_Port, D3_Pin},
		{D4_GPIO_Port, D4_Pin},
		{D5_GPIO_Port, D5_Pin},
		{D6_GPIO_Port, D6_Pin},
		{D7_GPIO_Port, D7_Pin},
		{D8_GPIO_Port, D8_Pin}

};

struct {
	bool enabled;
	uint8_t symbol;
} digit[DIGITS_COUNT] = {0};

uint8_t digit_current_index = 0;

void seven_segment_init(void){
	for(uint8_t i = 0; i < DIGITS_COUNT; i++){
		digit[i].enabled = false;
		digit[i].symbol = 0;
	}
}

void seven_segment_tick(void){
	/* Disable current digit */
	HAL_GPIO_WritePin(digit_hw[digit_current_index].GPIOx, digit_hw[digit_current_index].GPIO_Pin, RESET);
	digit_current_index++;
	if(digit_current_index >= DIGITS_COUNT){
		digit_current_index = 0;
	}

	/* Clear segments pins */
	GPIOA->ODR |= A_Pin|B_Pin|C_Pin|D_Pin|E_Pin|F_Pin|G_Pin|DP_Pin;

	/* Set code for next digit on segments port */
	if(digit[digit_current_index].enabled){
		GPIOA->ODR ^= digit[digit_current_index].symbol & (A_Pin|B_Pin|C_Pin|D_Pin|E_Pin|F_Pin|G_Pin|DP_Pin);
	}

	/* Enable current digit */
	HAL_GPIO_WritePin(digit_hw[digit_current_index].GPIOx, digit_hw[digit_current_index].GPIO_Pin, SET);
}

void seven_segment_enable(uint8_t index, bool state){
	digit[index].enabled = state;
}

bool seven_segment_is_enabled(uint8_t index){
	return digit[index].enabled;
}

void seven_segment_set_symbol(uint8_t index, uint8_t symbol){
	digit[index].symbol = symbol;
}

uint8_t seven_segment_get_symbol(uint8_t index){
	return digit[index].symbol;
}

void seven_segment_set_dot(uint8_t index, bool state){
	if(state){
		digit[index].symbol |= 0x80;
	}
	else{
		digit[index].symbol &= ~0x80;
	}
}

bool seven_segment_get_dot(uint8_t index){
	return (digit[index].symbol & 0x80)?(true):(false);
}

void seven_segment_set_number(uint8_t index, uint8_t number){
	if(number > 9){
		number = 9;
	}

	digit[index].symbol = digits[number];
}

