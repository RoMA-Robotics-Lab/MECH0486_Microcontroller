// Count rising A edges in one direction, with a 2 ms interrupt lockout.
// Encoder: CLK/A GP0, DT/B GP1, VCC 3V3, GND common.

#include <stdio.h>
#include "pico/stdlib.h"

#define ENC_A 0
#define ENC_B 1

#define DEBOUNCE_US 2000

volatile int count = 0;

void enc_isr(uint gpio, uint32_t events) {
    static uint64_t last = 0;
    uint64_t now = time_us_64();

    if (now - last < DEBOUNCE_US) return;
    last = now;

    count++;
}

int main() {
    stdio_init_all();
    gpio_init(ENC_A); gpio_set_dir(ENC_A, GPIO_IN); gpio_pull_up(ENC_A);
    gpio_init(ENC_B); gpio_set_dir(ENC_B, GPIO_IN); gpio_pull_up(ENC_B);

    gpio_set_irq_enabled_with_callback(ENC_A, GPIO_IRQ_EDGE_RISE, true, &enc_isr);

    printf("\n=== 엔코더 세어 보기 ===\n\n");

    int prev = count - 1;
    while (true) {
        int c = count;
        if (c != prev) {
            prev = c;
            printf("count = %d\n", c);
        }
    }
}
