// Run a clock and increment minutes on button presses (no debounce).
// TM1637: CLK GP4, DIO GP5, VCC 3V3. Button: GP0 to GND. LED: GP25.

#include <stdio.h>
#include "pico/stdlib.h"
#include "tm1637.h"

#define LED      25
#define BTN       0
#define SEG_CLK   4
#define SEG_DIO   5

int main() {
    stdio_init_all();
    gpio_init(LED); gpio_set_dir(LED, GPIO_OUT);
    gpio_init(BTN); gpio_set_dir(BTN, GPIO_IN); gpio_pull_up(BTN);

    tm1637_init(SEG_CLK, SEG_DIO);

    uint32_t sec = 0, min = 0, hour = 0;
    uint64_t next = time_us_64() + 1000000;
    bool prev = false;
    bool dirty = true;

    while (true) {

        if (time_us_64() >= next) {
            next += 1000000;
            gpio_xor_mask(1u << LED);
            if (++sec == 60) { sec = 0; if (++min == 60) { min = 0; if (++hour == 24) hour = 0; } }
            printf("%02u:%02u:%02u\n", hour, min, sec);
            dirty = true;
        }

        bool now = !gpio_get(BTN);
        if (now && !prev) { min = (min + 1) % 60; dirty = true; }
        prev = now;

        if (dirty) {
            tm1637_clock(hour, min, (sec % 2) == 0);
            dirty = false;
        }

    }
}
