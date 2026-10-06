// Compare a sleep-based clock with elapsed timer time.
// Onboard LED: GP25.

#include <stdio.h>
#include "pico/stdlib.h"

#define LED 25

int main() {
    stdio_init_all();
    gpio_init(LED); gpio_set_dir(LED, GPIO_OUT);

    uint32_t sec = 0, min = 0, hour = 0;
    uint32_t ticks = 0;
    uint64_t t0 = time_us_64();

    while (true) {
        gpio_put(LED, 1); sleep_ms(500);
        gpio_put(LED, 0); sleep_ms(500);

        ticks++;
        sec++;
        if (sec == 60) { sec = 0; min++; }
        if (min == 60) { min = 0; hour++; }
        if (hour == 24) hour = 0;

        uint64_t real_ms = (time_us_64() - t0) / 1000;
        long err = (long)real_ms - (long)ticks * 1000;

        printf("시계 %02u:%02u:%02u   실제 %lu.%03lu초   오차 %+ld ms\n",
               hour, min, sec,
               (unsigned long)(real_ms / 1000), (unsigned long)(real_ms % 1000),
               err);
    }
}
