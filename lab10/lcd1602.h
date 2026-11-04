// LCD1602 API for an HD44780 with a PCF8574 I2C backpack.

#ifndef LCD1602_H
#define LCD1602_H

#include <stdbool.h>
#include <stdint.h>
#include "hardware/i2c.h"

void lcd_init(i2c_inst_t *i2c, uint8_t addr);

void lcd_clear(void);
void lcd_home(void);
// Cursor: row 0-1, column 0-15.
void lcd_set_cursor(int row, int col);
void lcd_putc(char c);
void lcd_puts(const char *s);
void lcd_backlight(bool on);

#endif
