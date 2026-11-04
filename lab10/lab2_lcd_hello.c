// Show a greeting and uptime on an LCD1602.
// SDA GP0, SCL GP1, VCC 3V3, GND common. Verify LCD_ADDR first.

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "lcd1602.h"

#define LCD_ADDR 0x27

int main(void) {
    stdio_init_all();
    i2c_init(i2c0, 100000);
    gpio_set_function(0, GPIO_FUNC_I2C);
    gpio_set_function(1, GPIO_FUNC_I2C);
    gpio_pull_up(0); gpio_pull_up(1);
    lcd_init(i2c0, LCD_ADDR);
    lcd_backlight(true);
    lcd_set_cursor(0, 0); lcd_puts("Hello Pico");
    unsigned sec = 0;
    uint64_t next = time_us_64() + 1000000;
    while (true) {
        if (time_us_64() >= next) {
            next += 1000000;
            char line[17];
            snprintf(line, sizeof(line),
                     "uptime %8u ", sec++);
            lcd_set_cursor(1, 0);
            lcd_puts(line);
        }
        tight_loop_contents();
    }
}
