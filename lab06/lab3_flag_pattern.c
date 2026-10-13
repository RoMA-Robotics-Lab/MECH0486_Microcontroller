// Count presses in the ISR and print from main.
// Button: GP0 to GND (active-low). LED: GP1.

#include <stdio.h>
#include "pico/stdlib.h"

#define BTN 0
#define LED 1

volatile uint64_t last_us = 0;
volatile bool     pressed = false;
volatile uint32_t count   = 0;

void btn_isr(uint gpio, uint32_t events) {
    if (time_us_64() - last_us < 5000) return;
    last_us = time_us_64();
    count++; pressed = true;
    gpio_xor_mask(1u << LED);
}

int main() {
    stdio_init_all();
    gpio_init(BTN); gpio_set_dir(BTN, GPIO_IN); gpio_pull_up(BTN);
    gpio_init(LED); gpio_set_dir(LED, GPIO_OUT);
    gpio_set_irq_enabled_with_callback(BTN, GPIO_IRQ_EDGE_FALL, true, &btn_isr);

    printf("\n=== 플래그 패턴 ===\n버튼을 눌러보세요.\n\n");

    while (true) {
        if (pressed) {
            pressed = false;
            printf("눌린 횟수: %u\n", count);
        }

    }
}
