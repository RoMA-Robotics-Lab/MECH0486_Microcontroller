// Print the compile target and SDK-configured clock frequencies.

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"

int main() {
    stdio_init_all();

    while (true) {
        printf("\n===== 보드 정보 =====\n");

#if PICO_RP2350
        printf("칩          : RP2350 (Pico 2)\n");
        printf("데이터시트  : RP2350-datasheet.pdf\n");
#elif PICO_RP2040
        printf("칩          : RP2040 (Pico 1)\n");
        printf("데이터시트  : RP2040-datasheet.pdf\n");
#else
        printf("칩          : 알 수 없음\n");
#endif

        uint32_t f_sys  = clock_get_hz(clk_sys);
        uint32_t f_peri = clock_get_hz(clk_peri);
        uint32_t f_usb  = clock_get_hz(clk_usb);

        printf("clk_sys     : %u Hz (%.1f MHz)\n", f_sys,  f_sys  / 1e6);
        printf("clk_peri    : %u Hz (%.1f MHz)\n", f_peri, f_peri / 1e6);
        printf("clk_usb     : %u Hz (%.1f MHz)\n", f_usb,  f_usb  / 1e6);

        printf("clk_sys 1주기: %.2f ns\n", 1e9 / f_sys);

        sleep_ms(3000);
    }
}
