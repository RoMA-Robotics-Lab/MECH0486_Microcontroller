// Show a greeting and counter on an SSD1306 OLED.
// SDA GP0, SCL GP1, VCC 3V3, GND common. Check address 0x3C/0x3D.
// Keep the bus at 100 kHz when sharing it with a PCF8574 LCD.

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd1306.h"

int main(void) {
    stdio_init_all();
    i2c_init(i2c0, 100000);
    gpio_set_function(0, GPIO_FUNC_I2C);
    gpio_set_function(1, GPIO_FUNC_I2C);
    gpio_pull_up(0); gpio_pull_up(1);
    ssd1306_init(i2c0, 0x3C);
    unsigned count = 0;
    uint64_t next = time_us_64();
    while (true) {
        if (time_us_64() >= next) {
            next += 1000000;
            char line[SSD1306_COLS + 1];
            snprintf(line, sizeof(line), "COUNT %u", count++);
            ssd1306_clear();
            ssd1306_text(0, 0, "HELLO OLED");
            ssd1306_text(0, 3, line);
            ssd1306_show();
        }
        tight_loop_contents();
    }
}
