// Cycle LEDs on GP1, GP2, and GP3.
// Connect each LED to GND through a 220-330 ohm series resistor.

#include "pico/stdlib.h"

#define LED_R  1
#define LED_G  2
#define LED_Y  3

int main() {

    gpio_init(LED_R);  gpio_set_dir(LED_R, GPIO_OUT);
    gpio_init(LED_G);  gpio_set_dir(LED_G, GPIO_OUT);
    gpio_init(LED_Y);  gpio_set_dir(LED_Y, GPIO_OUT);

    while (true) {
        gpio_put(LED_R, 1); sleep_ms(300); gpio_put(LED_R, 0);
        gpio_put(LED_G, 1); sleep_ms(300); gpio_put(LED_G, 0);
        gpio_put(LED_Y, 1); sleep_ms(300); gpio_put(LED_Y, 0);
    }
}
