/*
 * 3주차 실습 3 (§6.3) — 변주 ②: 3비트 이진 카운터
 *
 * LED 5개 중 앞 3개(GP0~2)로 count(0~7)의 하위 3비트를 표시한다.
 * 이진수를 눈으로 보는 실습.
 *
 * 배선: 실습 1과 동일 (GP0~2만 사용, GP3·4는 그대로 둔다)
 */

#include "pico/stdlib.h"

#define LEDS  0b111u   // GP0,1,2 (하위 3개 LED)

int main() {
    gpio_init_mask(LEDS);
    gpio_set_dir_out_masked(LEDS);

    uint8_t count = 0;
    while (true) {
        gpio_put_masked(LEDS, count);  // count의 하위 3비트를 GP0~2에 한 번에 출력
        count = (count + 1) & 0b111;   // 0~7 순환 (마스크로 wrap)
        sleep_ms(500);
    }
}
