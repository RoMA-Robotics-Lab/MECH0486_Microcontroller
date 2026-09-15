/*
 * 2주차 실습 2 — 디지털 입력 (택트 버튼)
 *
 * 버튼을 누르고 있는 동안 LED가 켜진다.
 *
 * 배선 (키트 L5 Switch Light)
 *   GP0 → 버튼 Pin2,  GND → 버튼 Pin1   ※ 내부 풀업 사용 → 눌림 = LOW
 *   GP1 → LED (+),    GND → LED (-)
 */

#include "pico/stdlib.h"

#define BUTTON_PIN 0        // 택트 버튼 (키트 L5: GP0, active-low)
#define LED_R      1        // LED (키트 L5: GP1)

int main() {
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);     // 입력으로 설정
    gpio_pull_up(BUTTON_PIN);              // 내부 풀업 켜기 (부유 방지)

    gpio_init(LED_R);
    gpio_set_dir(LED_R, GPIO_OUT);

    while (true) {
        // 풀업이라 평소 1(HIGH), 눌리면 0(LOW) → 반전해서 "눌림=1"로
        bool pressed = !gpio_get(BUTTON_PIN);
        gpio_put(LED_R, pressed);          // 읽은 값을 그대로 LED에
        sleep_ms(10);
    }
}
