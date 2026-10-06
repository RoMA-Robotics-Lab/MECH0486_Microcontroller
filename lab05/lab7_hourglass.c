// Start a 10-second countdown, then beep and blink the display.
// TM1637: CLK GP4, DIO GP5, VCC 3V3. Button: GP0 to GND.
// Passive buzzer: GP14 to GND. LED: GP25.
// Countdown and completion feedback block other input handling.

#include <stdio.h>
#include "pico/stdlib.h"
#include "tm1637.h"

#define BTN      0
#define BUZZER  14
#define LED     25
#define SEG_CLK  4
#define SEG_DIO  5

#define START_SEC 10

void beep(uint pin, uint freq, uint ms) {
    uint half_us = 500000u / freq;
    uint cycles  = (ms * 1000u) / (half_us * 2u);
    for (uint i = 0; i < cycles; i++) {
        gpio_put(pin, 1); sleep_us(half_us);
        gpio_put(pin, 0); sleep_us(half_us);
    }
}

int main() {
    stdio_init_all();
    gpio_init(BTN);    gpio_set_dir(BTN, GPIO_IN); gpio_pull_up(BTN);
    gpio_init(BUZZER); gpio_set_dir(BUZZER, GPIO_OUT);
    gpio_init(LED);    gpio_set_dir(LED, GPIO_OUT);

    tm1637_init(SEG_CLK, SEG_DIO);
    tm1637_number(START_SEC);

    while (true) {
        if (!gpio_get(BTN)) {
            sleep_ms(20);
            int remain = START_SEC;
            uint64_t next = time_us_64() + 1000000;

            tm1637_number(remain);
            while (remain > 0) {
                if (time_us_64() >= next) {
                    next += 1000000;
                    remain--;
                    gpio_xor_mask(1u << LED);
                    tm1637_number(remain);
                    printf("남은 시간: %d\n", remain);
                }
            }
            beep(BUZZER, 2000, 500);

            for (int i = 0; i < 6; i++) {
                (i % 2) ? tm1637_number(0) : tm1637_clear();
                sleep_ms(150);
            }
            tm1637_number(START_SEC);

            while (!gpio_get(BTN)) tight_loop_contents();
        }
    }
}
