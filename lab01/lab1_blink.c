/*
 * 1주차 실습 (§5.1) — 첫 프로그램: Blink
 *
 * 온보드 LED를 0.5초 간격으로 깜빡인다.
 *
 * 배선: 없음. 키트는 이 수업 끝에 받으므로, 오늘은 보드만 USB에 꽂으면 된다.
 *
 * 짚어야 할 네 가지 (강의안 §5.2)
 *   1. while(true)  - MCU에는 돌아갈 OS가 없다. main이 끝나면 갈 곳이 없다.
 *                     그래서 전원이 꺼질 때까지 무한히 돈다(슈퍼 루프).
 *   2. gpio_init()  - 핀 하나가 GPIO/UART/I2C/PWM 여러 기능으로 쓰이므로
 *                     "이 핀을 GPIO로 쓰겠다"고 지정해야 한다.
 *   3. gpio_set_dir - LED를 켜려면 전압을 내보내야 하므로 출력.
 *                     이걸 빼먹으면 기본이 입력이라 LED가 안 켜진다(에러도 없이).
 *   4. sleep_ms()   - 이 500ms 동안 CPU는 아무것도 못 한다.
 *                     5주차(타이머)·6주차(인터럽트)에서 이 한계를 푼다.
 *
 * 확인 과제 (강의안 §5.4)
 *   sleep_ms 값을 바꿔가며 관찰한다. README의 「확인 과제」 절 참고.
 */

#include "pico/stdlib.h"

#define LED_PIN 25          // Pico 보드의 온보드 LED는 GP25에 연결

int main() {
    gpio_init(LED_PIN);                    // 이 핀을 GPIO로 사용하겠다고 초기화
    gpio_set_dir(LED_PIN, GPIO_OUT);       // 방향 설정: 출력

    while (true) {                          // 임베디드 프로그램은 끝나지 않는다
        gpio_put(LED_PIN, 1);              // HIGH (3.3V) -> LED 켜짐
        sleep_ms(500);                      // 500ms 대기
        gpio_put(LED_PIN, 0);              // LOW (0V) -> LED 꺼짐
        sleep_ms(500);
    }
}
