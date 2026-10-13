// Decode encoder transitions using an A/B state table.
// Encoder: CLK/A GP0, DT/B GP1, VCC 3V3, GND common.

#include <stdio.h>
#include "pico/stdlib.h"

#define ENC_A 0
#define ENC_B 1

// Transitions per detent; adjust for the encoder.
#define ENC_STEPS 1

// Index: (previous << 2) | current. Zero means no valid step.
static const int8_t QTAB[16] = {
     0, -1, +1,  0,
    +1,  0,  0, -1,
    -1,  0,  0, +1,
     0, +1, -1,  0,
};

volatile int count = 0;

static uint8_t prev_state = 0;
static int8_t  sub        = 0;

void enc_isr(uint gpio, uint32_t events) {
    uint8_t cur = (gpio_get(ENC_A) << 1) | gpio_get(ENC_B);
    int8_t  d   = QTAB[(prev_state << 2) | cur];

    if (d == 0) return;
    prev_state = cur;

    sub += d;
    if (sub >= ENC_STEPS)       { sub = 0; count++; }
    else if (sub <= -ENC_STEPS) { sub = 0; count--; }
}

int main() {
    stdio_init_all();
    gpio_init(ENC_A); gpio_set_dir(ENC_A, GPIO_IN); gpio_pull_up(ENC_A);
    gpio_init(ENC_B); gpio_set_dir(ENC_B, GPIO_IN); gpio_pull_up(ENC_B);

    prev_state = (gpio_get(ENC_A) << 1) | gpio_get(ENC_B);

    gpio_set_irq_enabled_with_callback(ENC_A,
            GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &enc_isr);
    gpio_set_irq_enabled(ENC_B, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);

    printf("\n=== 엔코더 (두 상으로 읽기) ===\n\n");

    int prev = count - 1;
    while (true) {
        int c = count;
        if (c != prev) {
            prev = c;
            printf("count = %d\n", c);
        }
    }
}
