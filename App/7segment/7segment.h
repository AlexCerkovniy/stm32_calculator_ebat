#pragma once

#include <stdint.h>
#include <stdbool.h>

#define DIGITS_COUNT		(8)

void seven_segment_init(void);
void seven_segment_tick(void);
void seven_segment_enable(uint8_t index, bool state);
bool seven_segment_is_enabled(uint8_t index);
void seven_segment_set_symbol(uint8_t index, uint8_t symbol);
uint8_t seven_segment_get_symbol(uint8_t index);
void seven_segment_set_dot(uint8_t index, bool state);
bool seven_segment_get_dot(uint8_t index);
void seven_segment_set_number(uint8_t index, uint8_t number);
