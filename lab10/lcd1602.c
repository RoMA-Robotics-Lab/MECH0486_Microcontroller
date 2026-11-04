// HD44780 LCD driver using a PCF8574 I2C backpack.
// P0 RS, P1 RW, P2 EN, P3 backlight, P4-P7 data; send high nibble first.

#include "lcd1602.h"
#include "pico/stdlib.h"

#define BIT_RS 0x01
#define BIT_RW 0x02
#define BIT_EN 0x04
#define BIT_BL 0x08

#define CMD_CLEAR       0x01
#define CMD_HOME        0x02
#define CMD_ENTRY_MODE  0x06
#define CMD_DISPLAY_ON  0x0C
#define CMD_FUNCTION    0x28

static i2c_inst_t *s_i2c;
static uint8_t     s_addr;
static uint8_t     s_backlight = BIT_BL;

static void pcf_write(uint8_t data) {
    uint8_t b = data | s_backlight;
    i2c_write_blocking(s_i2c, s_addr, &b, 1, false);
}

static void strobe(uint8_t data) {
    pcf_write(data | BIT_EN);
    sleep_us(1);
    pcf_write(data & ~BIT_EN);
    sleep_us(50);
}

static void write_nibble(uint8_t nibble, uint8_t mode) {
    strobe((nibble & 0xF0) | mode);
}

static void write_byte(uint8_t value, uint8_t mode) {
    write_nibble(value & 0xF0, mode);
    write_nibble((value << 4) & 0xF0, mode);
}

static void lcd_command(uint8_t cmd) { write_byte(cmd, 0); }

void lcd_init(i2c_inst_t *i2c, uint8_t addr) {
    s_i2c = i2c;
    s_addr = addr;

    sleep_ms(50);

    write_nibble(0x30, 0); sleep_ms(5);
    write_nibble(0x30, 0); sleep_us(150);
    write_nibble(0x30, 0); sleep_us(150);
    write_nibble(0x20, 0); sleep_us(150);

    lcd_command(CMD_FUNCTION);
    lcd_command(CMD_DISPLAY_ON);
    lcd_command(CMD_ENTRY_MODE);
    lcd_clear();
}

void lcd_clear(void) { lcd_command(CMD_CLEAR); sleep_ms(2); }
void lcd_home(void)  { lcd_command(CMD_HOME);  sleep_ms(2); }

void lcd_set_cursor(int row, int col) {
    static const uint8_t row_base[2] = { 0x00, 0x40 };
    if (row < 0) row = 0;
    if (row > 1) row = 1;
    lcd_command(0x80 | (row_base[row] + col));
}

void lcd_putc(char c)          { write_byte((uint8_t)c, BIT_RS); }
void lcd_puts(const char *s)   { while (*s) lcd_putc(*s++); }

void lcd_backlight(bool on) {
    s_backlight = on ? BIT_BL : 0;
    pcf_write(0);
}
