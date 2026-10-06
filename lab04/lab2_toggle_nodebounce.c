// Toggle on each press. No debounce: contact bounce may cause extra toggles.
// Button: GP0 to GND (active-low). LED: GP1.

#include "pico/stdlib.h"

#define BTN 0
#define LED 1

int main() {
    gpio_init(BTN); gpio_set_dir(BTN, GPIO_IN); gpio_pull_up(BTN);
    gpio_init(LED); gpio_set_dir(LED, GPIO_OUT);

    bool led = false, prev = false;
    while (true) {
        bool now = !gpio_get(BTN);
        if (now && !prev) {
            led = !led;
            gpio_put(LED, led);
        }
        prev = now;

    }
}
