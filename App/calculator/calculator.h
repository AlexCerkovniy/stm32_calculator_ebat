#pragma once

#include <stdint.h>
#include <stdbool.h>

#define ERROR_MSG_SHOW_TIME_MS		(1000)

typedef enum {
	CALC_OP_NONE = 0,
	CALC_OP_ADD,
	CALC_OP_SUBSTRACT,
	CALC_OP_MULTIPLY,
	CALC_OP_DIVIDE
} calc_operation_t;

typedef struct {
	uint32_t absolute;
	uint8_t absolute_digits;
	uint32_t fraction;
	uint8_t fraction_digits;
	bool negative;
} calc_number_t;

void calculator_init(void);
void calculator_main(void);
void calculator_tick(uint16_t period);
