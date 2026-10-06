// Print button state over USB serial and mirror it on the LED.
// Button: GP0 to GND (active-low). LED: GP1.

#include <stdio.h>
#include "pico/stdlib.h"

#define BUTTON_PIN 0
#define LED_R      1

int main() {
    stdio_init_all();

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    gpio_init(LED_R);
    gpio_set_dir(LED_R, GPIO_OUT);

    printf("\n=== 버튼 값 보기 ===\n");
    printf("버튼 GP%d, LED GP%d\n", BUTTON_PIN, LED_R);
    printf("버튼을 눌러보세요.\n\n");

    int tick = 0;

    while (true) {
        int raw = gpio_get(BUTTON_PIN);
        bool pressed = !raw;

        gpio_put(LED_R, pressed);

        // Report every 200 ms.
        if (++tick >= 20) {
            tick = 0;
            printf("gpio_get() = %d   ->  %s\n", raw, pressed ? "눌림" : "안 눌림");
        }

        sleep_ms(10);
    }
}
