/*
 * 2주차 실습 1 — 키트 LED 켜기
 *
 * 온보드 LED(GP25)가 아니라 브레드보드의 단색 LED 3개를 순서대로 켠다.
 *
 * 배선 (키트 L3 Colorful Flowing Light 변형)
 *   GP1 → LED_R (+),  GP2 → LED_G (+),  GP3 → LED_Y (+)
 *   GND → 각 LED (-)   ※ 220~330옴 전류제한 저항 직렬
 *
 * GP0은 실습 2·3의 버튼용으로 비워둔다.
 */

#include "pico/stdlib.h"

#define LED_R  1
#define LED_G  2
#define LED_Y  3

int main() {
    // 세 개의 LED를 모두 출력으로 설정
    gpio_init(LED_R);  gpio_set_dir(LED_R, GPIO_OUT);
    gpio_init(LED_G);  gpio_set_dir(LED_G, GPIO_OUT);
    gpio_init(LED_Y);  gpio_set_dir(LED_Y, GPIO_OUT);

    while (true) {
        gpio_put(LED_R, 1); sleep_ms(300); gpio_put(LED_R, 0);
        gpio_put(LED_G, 1); sleep_ms(300); gpio_put(LED_G, 0);
        gpio_put(LED_Y, 1); sleep_ms(300); gpio_put(LED_Y, 0);
    }
}
