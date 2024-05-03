#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef enum {
	CALC_OP_NONE = 0,
	CALC_OP_ADD,
	CALC_OP_SUBSTRACT,
	CALC_OP_MULTIPLY,
	CALC_OP_DIVIDE
} calc_operation_t;

void calculator_init(void);
void calculator_main(void);
