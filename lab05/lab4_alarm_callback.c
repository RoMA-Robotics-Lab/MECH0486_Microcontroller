// Update seconds in a repeating timer callback; print from main.
// Onboard LED: GP25.

#include <stdio.h>
#include "pico/stdlib.h"

#define LED 25

volatile uint32_t sec = 0;

bool tick(repeating_timer_t *rt) {
    sec++;
    gpio_xor_mask(1u << LED);
    return true;
}

int main() {
    stdio_init_all();
    gpio_init(LED); gpio_set_dir(LED, GPIO_OUT);

    repeating_timer_t t;
    add_repeating_timer_ms(1000, tick, NULL, &t);

    uint32_t last_shown = 0;
    while (true) {

        uint32_t s = sec;
        if (s != last_shown) { last_shown = s; printf("sec = %u\n", s); }
        tight_loop_contents();
    }
}
