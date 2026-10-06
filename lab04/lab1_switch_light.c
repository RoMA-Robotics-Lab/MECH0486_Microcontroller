// Light the LED while the button is held.
// Button: GP0 to GND (active-low). LED: GP1.

#include "pico/stdlib.h"

#define BTN 0
#define LED 1

int main() {
    gpio_init(BTN); gpio_set_dir(BTN, GPIO_IN); gpio_pull_up(BTN);
    gpio_init(LED); gpio_set_dir(LED, GPIO_OUT);
    while (true) {
        gpio_put(LED, !gpio_get(BTN));
        sleep_ms(5);
    }
}
