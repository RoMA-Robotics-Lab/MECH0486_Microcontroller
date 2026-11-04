// SSD1306 128x64 API; drawing changes RAM until ssd1306_show().
// Font: A-Z and 0-9; lowercase becomes uppercase, other characters are blank.

#ifndef SSD1306_H_
#define SSD1306_H_

#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define SSD1306_WIDTH  128
#define SSD1306_HEIGHT  64
#define SSD1306_COLS    16
#define SSD1306_ROWS     8

void ssd1306_init(i2c_inst_t *i2c, uint8_t addr);

// Clear the framebuffer only.
void ssd1306_clear(void);

// Transfer the framebuffer to the display (blocking).
void ssd1306_show(void);

// Text position: column 0-15, row 0-7.
void ssd1306_text(uint col, uint row, const char *s);

void ssd1306_pixel(uint x, uint y, bool on);

// Toggle display output without clearing RAM.
void ssd1306_display_on(bool on);

#endif
