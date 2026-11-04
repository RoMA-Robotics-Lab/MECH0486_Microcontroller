// Scan I2C addresses 0x08-0x77 using one-byte reads.
// SDA GP0, SCL GP1, VCC 3V3, GND common; wire with power off.
// Read-only scanning may miss write-only devices such as SSD1306.

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

int main(void) {
    stdio_init_all();
    i2c_init(i2c0, 100000);
    gpio_set_function(0, GPIO_FUNC_I2C);
    gpio_set_function(1, GPIO_FUNC_I2C);
    gpio_pull_up(0); gpio_pull_up(1);
    sleep_ms(2000);
    while (true) {
        unsigned found = 0;
        printf("\nI2C scan\n");

        for (uint8_t addr = 0x08; addr < 0x78; addr++) {
            uint8_t dummy;
            int n = i2c_read_timeout_us(
                i2c0, addr, &dummy, 1, false, 10000);
            if (n == 1) {
                printf("ACK: 0x%02X\n", addr);
                found++;
            }
        }
        printf("Responding addresses: %u\n", found);
        sleep_ms(3000);
    }
}
