/*
 * 3주차 실습 2 (§6.2) — 변주 ①: 같은 흐르는 불빛을 비트 마스크로
 *
 * 실습 1과 동작은 같지만, 켜진 비트 하나를 왼쪽으로 밀어(1u << i) 구현한다.
 * LED가 20개여도 N만 바꾸면 된다.
 *
 * 배선: 실습 1과 동일 (GP0~4)
 */

#include "pico/stdlib.h"

#define N    5
#define LEDS ((1u << N) - 1)        // 0b11111 = GP0~4 마스크

int main() {
    gpio_init_mask(LEDS);
    gpio_set_dir_out_masked(LEDS);

    while (true) {
        for (int i = 0; i < N; i++) {
            gpio_put_masked(LEDS, 1u << i);   // i번째 비트만 1 → 그 LED만 켜짐
            sleep_ms(150);
        }
    }
}
