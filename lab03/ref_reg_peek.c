/*
 * 3주차 실습 6 (§6.2 심화, 선택) — 레지스터를 눈으로 보기
 *
 * "비트를 다룬다"는 말이 잘 와닿지 않을 때, 출력 레지스터 값을 직접 찍어본다.
 * 세트/클리어할 때마다 비트 0이 0↔1 하는 것이 보인다.
 *
 * 배선: GP0에 LED 하나만 있으면 된다 (실습 1 배선 그대로 써도 됨)
 *
 * 보는 법
 *   보드를 USB로 연결한 뒤 시리얼 모니터를 연다.
 *   VS Code라면 Serial Monitor 확장에서 해당 COM 포트를 열면 된다.
 *
 * 디버그 프로브가 있다면 VS Code 디버거의 레지스터(SFR) 뷰로도 실시간
 * 관찰할 수 있다. 없으면 아래 printf 방식으로 충분하다.
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/structs/sio.h"   // sio_hw 접근에 필요

int main() {
    stdio_init_all();               // USB 시리얼 켜기

    gpio_init(0);
    gpio_set_dir(0, GPIO_OUT);

    while (true) {
        gpio_set_mask(1u << 0);
        printf("set  -> gpio_out = 0x%08x\n", sio_hw->gpio_out);  // 비트 0이 1
        sleep_ms(1000);

        gpio_clr_mask(1u << 0);
        printf("clr  -> gpio_out = 0x%08x\n", sio_hw->gpio_out);  // 비트 0이 0
        sleep_ms(1000);
    }
}
