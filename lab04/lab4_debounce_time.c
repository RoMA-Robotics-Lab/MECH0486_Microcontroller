// Toggle after a 20 ms press check, then wait for release (blocking).
// Button: GP0 to GND (active-low). LED: GP1.

#include "pico/stdlib.h"

#define BTN 0
#define LED 1

int main() {
    gpio_init(BTN); gpio_set_dir(BTN, GPIO_IN); gpio_pull_up(BTN);
    gpio_init(LED); gpio_set_dir(LED, GPIO_OUT);

    bool led = false;
    while (true) {
        if (!gpio_get(BTN)) {
            sleep_ms(20);
            if (!gpio_get(BTN)) {
                led = !led;
                gpio_put(LED, led);

                // Wait for release.
                while (!gpio_get(BTN)) tight_loop_contents();
            }
        }
    }
}
