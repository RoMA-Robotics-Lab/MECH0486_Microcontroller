// Toggle the LED on falling-edge button interrupts; no debounce.
// Button: GP0 to GND (active-low). LED: GP1.

#include "pico/stdlib.h"

#define BTN 0
#define LED 1

void gpio_isr(uint gpio, uint32_t events) {
    if (gpio == BTN) {
        gpio_xor_mask(1u << LED);
    }
}

int main() {
    gpio_init(BTN); gpio_set_dir(BTN, GPIO_IN); gpio_pull_up(BTN);
    gpio_init(LED); gpio_set_dir(LED, GPIO_OUT);

    gpio_set_irq_enabled_with_callback(BTN, GPIO_IRQ_EDGE_FALL, true, &gpio_isr);

    while (true) {
        tight_loop_contents();
    }
}
