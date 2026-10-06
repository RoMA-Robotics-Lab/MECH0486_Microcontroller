// Display a clock starting at 23:58:55.
// TM1637: CLK GP4, DIO GP5, VCC 3V3, GND common. LED: GP25.

#include <stdio.h>
#include "pico/stdlib.h"
#include "tm1637.h"

#define SEG_CLK  4
#define SEG_DIO  5
#define LED     25

int main() {
    stdio_init_all();

    gpio_init(LED);
    gpio_set_dir(LED, GPIO_OUT);

    tm1637_init(SEG_CLK, SEG_DIO);

    uint32_t sec = 55, min = 58, hour = 23;

    uint64_t next = time_us_64() + 1000000;

    tm1637_clock(hour, min, true);

    while (true) {
        if (time_us_64() >= next) {
            next += 1000000;

            if (++sec == 60) { sec = 0; if (++min == 60) { min = 0; if (++hour == 24) hour = 0; } }

            bool colon = (sec % 2) == 0;
            gpio_put(LED, colon);

            tm1637_clock(hour, min, colon);

            printf("%02u:%02u:%02u\n", hour, min, sec);
        }

    }
}
