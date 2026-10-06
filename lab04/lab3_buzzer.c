// Play three tones on a passive buzzer.
// Buzzer: GP14 (+), GND (-).

#include "pico/stdlib.h"

#define BUZZER 14

// Blocking square wave; freq in Hz, duration in ms.
void beep(uint pin, uint freq, uint ms) {
    uint half_us = 500000u / freq;
    uint cycles  = (ms * 1000u) / (half_us * 2u);
    for (uint i = 0; i < cycles; i++) {
        gpio_put(pin, 1); sleep_us(half_us);
        gpio_put(pin, 0); sleep_us(half_us);
    }
}

int main() {
    gpio_init(BUZZER);
    gpio_set_dir(BUZZER, GPIO_OUT);

    while (true) {
        beep(BUZZER, 1000, 200);
        sleep_ms(200);
        beep(BUZZER, 2000, 200);
        sleep_ms(200);
        beep(BUZZER, 3000, 200);
        sleep_ms(1000);
    }
}
