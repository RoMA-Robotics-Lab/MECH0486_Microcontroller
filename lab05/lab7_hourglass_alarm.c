// Timer-callback countdown with display and buzzer feedback.
// TM1637: CLK GP4, DIO GP5, VCC 3V3, GND common.
// Button: GP0 to GND. Passive buzzer: GP14 to GND. LED: GP25.

#include <stdio.h>
#include "pico/stdlib.h"
#include "tm1637.h"

#define BTN      0
#define BUZZER  14
#define LED     25
#define SEG_CLK  4
#define SEG_DIO  5

#define START_SEC 10

// Blocking tone: frequency in Hz, duration in ms.
void beep(uint pin, uint freq, uint ms) {
    uint half_us = 500000u / freq;
    uint cycles  = (ms * 1000u) / (half_us * 2u);
    for (uint i = 0; i < cycles; i++) {
        gpio_put(pin, 1); sleep_us(half_us);
        gpio_put(pin, 0); sleep_us(half_us);
    }
}

volatile int remain = 0;

// Update the countdown in the callback; handle output in main.
bool on_timer(repeating_timer_t *rt) {
    if (remain > 0) {
        remain--;
        gpio_xor_mask(1u << LED);
    }
    return true;
}

int main() {
    stdio_init_all();
    gpio_init(BTN);    gpio_set_dir(BTN, GPIO_IN); gpio_pull_up(BTN);
    gpio_init(BUZZER); gpio_set_dir(BUZZER, GPIO_OUT);
    gpio_init(LED);    gpio_set_dir(LED, GPIO_OUT);

    tm1637_init(SEG_CLK, SEG_DIO);
    tm1637_number(START_SEC);

    repeating_timer_t timer;
    add_repeating_timer_ms(1000, on_timer, NULL, &timer);

    int  prev     = -1;
    bool prev_btn = false;

    while (true) {

        // Active-low press edge; no debounce.
        bool now = !gpio_get(BTN);
        if (now && !prev_btn) remain = START_SEC;
        prev_btn = now;

        int r = remain;
        if (r != prev) {
            prev = r;
            tm1637_number(r);
            printf("남은 시간: %d\n", r);

            // Completion feedback blocks button polling.
            if (r == 0) {
                beep(BUZZER, 2000, 500);
                for (int i = 0; i < 6; i++) {
                    (i % 2) ? tm1637_number(0) : tm1637_clear();
                    sleep_ms(150);
                }
                tm1637_number(START_SEC);
            }
        }
    }
}
