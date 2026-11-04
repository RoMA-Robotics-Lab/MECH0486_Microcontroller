// Echo USB CDC input as characters and hex bytes; no UART wiring.
// Poll at most one byte every 50 ms.

#include <stdio.h>
#include "pico/stdlib.h"

int main(void) {
    stdio_init_all();
    while (true) {
        int c = getchar_timeout_us(0);
        if (c != PICO_ERROR_TIMEOUT)
            printf("RX: %c (0x%02X)\n", c, c);
        sleep_ms(50);
    }
}
