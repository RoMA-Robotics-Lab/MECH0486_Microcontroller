/*
 * 2주차 실습 3 (추가) — 시리얼로 버튼 값 보기
 *
 * 실습 1·2에서는 LED가 유일한 확인 수단이었다. LED가 안 켜지면 버튼이
 * 문제인지, 핀 번호가 틀렸는지, LED 배선이 문제인지 가릴 수 없다.
 *
 * printf로 값을 찍으면 "지금 버튼이 몇으로 읽히는지"를 직접 볼 수 있다.
 * 이것이 임베디드의 첫 디버깅 도구다.
 *
 * 동작은 실습 2와 같다. gpio_get()의 날것의 값을 함께 찍는 것만 다르다.
 *
 * 배선: 실습 2와 동일
 *   GP0 ↔ 버튼 ↔ GND        내부 풀업, 눌림 = LOW
 *   GP1 → LED (+), GND → LED (-)
 *
 * 보는 법
 *   1. build/lab3_button_serial.uf2 를 보드에 복사
 *   2. 복사가 끝나면 보드가 재부팅된다. BOOTSEL을 누르지 말고 그냥 둔다
 *   3. VS Code의 Serial Monitor 확장에서 새로 생긴 COM 포트를 연다
 *
 * ⚠️ 모니터를 늦게 열면 시작 메시지는 이미 지나가 안 보인다.
 *    버튼을 눌러보면 그때부터 값이 계속 찍힌다.
 */

#include <stdio.h>
#include "pico/stdlib.h"

#define BUTTON_PIN 0
#define LED_R      1

int main() {
    stdio_init_all();              // 시리얼 출력 준비

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
        int raw = gpio_get(BUTTON_PIN);   // 핀에서 읽은 날것의 값 (0 또는 1)
        bool pressed = !raw;              // 풀업이라 눌림 = 0 → 반전

        gpio_put(LED_R, pressed);         // LED로도 확인

        // 매 루프마다 찍으면 화면이 넘치므로 200ms에 한 번만 찍는다
        if (++tick >= 20) {
            tick = 0;
            printf("gpio_get() = %d   ->  %s\n", raw, pressed ? "눌림" : "안 눌림");
        }

        sleep_ms(10);
    }
}
