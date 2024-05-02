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
	uint8_t code;
} digit[DIGITS_COUNT] = {0};

uint8_t digit_current_index = 0;

void seven_segment_init(void){
	for(uint8_t i = 0; i < DIGITS_COUNT; i++){
		digit[i].enabled = true;
		digit[i].code = digits[9];
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
		GPIOA->ODR ^= digit[digit_current_index].code & (A_Pin|B_Pin|C_Pin|D_Pin|E_Pin|F_Pin|G_Pin|DP_Pin);
	}

	/* Enable current digit */
	HAL_GPIO_WritePin(digit_hw[digit_current_index].GPIOx, digit_hw[digit_current_index].GPIO_Pin, SET);
}
