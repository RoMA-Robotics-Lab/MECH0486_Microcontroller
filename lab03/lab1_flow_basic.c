/*
 * 3주차 실습 1 (§6.1) — 기본 실험: 흐르는 불빛 (키트 L3 방식)
 *
 * LED를 배열에 담아 하나씩 순서대로 켠다. 키트 L3 Colorful Flowing Light와
 * 동일한 동작이며, 비트 연산을 쓰지 않은 "비교 대상" 코드다.
 *
 * 배선 (키트 L3 그대로)
 *   GP0~GP4 → 각 LED (+),  GND → 각 LED (-)   ※ active-high, 저항 없이 직결
 */

#include "pico/stdlib.h"

#define N 5
const uint LED[N] = {0, 1, 2, 3, 4};   // GP0~4 (키트 L3 배선)

int main() {
    for (int i = 0; i < N; i++) { gpio_init(LED[i]); gpio_set_dir(LED[i], GPIO_OUT); }
    while (true) {
        for (int i = 0; i < N; i++) {
            gpio_put(LED[i], 1); sleep_ms(150);   // i번째 켜고
            gpio_put(LED[i], 0);                  // 끄고 다음으로
        }
    }
}
