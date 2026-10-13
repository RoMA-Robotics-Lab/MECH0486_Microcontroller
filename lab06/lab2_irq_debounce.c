// Toggle the LED with a 5 ms interrupt lockout.
// Button: GP0 to GND (active-low). LED: GP1.

#include "pico/stdlib.h"

#define BTN 0
#define LED 1

volatile uint64_t last_us = 0;

void btn_isr(uint gpio, uint32_t events) {
    uint64_t now = time_us_64();
    if (now - last_us < 5000) return;
    last_us = now;
    gpio_xor_mask(1u << LED);
}

int main() {
    gpio_init(BTN); gpio_set_dir(BTN, GPIO_IN); gpio_pull_up(BTN);
    gpio_init(LED); gpio_set_dir(LED, GPIO_OUT);
    gpio_set_irq_enabled_with_callback(BTN, GPIO_IRQ_EDGE_FALL, true, &btn_isr);
    while (true) tight_loop_contents();
}
