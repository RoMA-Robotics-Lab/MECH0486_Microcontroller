// Light the LED while the button is held.
// Button: GP0 to GND (active-low). LED: GP1.

#include "pico/stdlib.h"

#define BUTTON_PIN 0
#define LED_R      1

int main() {
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    gpio_init(LED_R);
    gpio_set_dir(LED_R, GPIO_OUT);

    while (true) {

        bool pressed = !gpio_get(BUTTON_PIN);
        gpio_put(LED_R, pressed);
        sleep_ms(10);
    }
}
