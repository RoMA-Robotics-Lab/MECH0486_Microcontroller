// TM1637 display API. Call tm1637_init() before other functions.

#ifndef TM1637_H
#define TM1637_H

#include "pico/stdlib.h"

void tm1637_init(uint clk_pin, uint dio_pin);

// Brightness: 0-7, clamped. Default: 7.
void tm1637_brightness(uint level);

void tm1637_clear(void);

// Right-aligned integer, clamped to -999..9999.
void tm1637_number(int num);

// Two two-digit values with an optional colon.
void tm1637_clock(uint left, uint right, bool colon);

// Raw segment bytes for the four digits.
void tm1637_raw(const uint8_t seg[4]);

#endif
