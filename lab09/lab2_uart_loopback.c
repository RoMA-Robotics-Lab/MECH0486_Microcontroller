// Send UART0 bytes and check the loopback response; log over USB.
// Connect GP0 (TX) to GP1 (RX) with power disconnected.

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

static void loopback(void);

int main(void) {
    stdio_init_all();
    uart_init(uart0, 115200);
    gpio_set_function(0, GPIO_FUNC_UART);
    gpio_set_function(1, GPIO_FUNC_UART);
    while (true) {
        loopback();
        sleep_ms(2000);
    }
}

static void loopback(void) {
    const char *msg = "Hello UART!";
    for (const char *p = msg; *p; ++p) {
        uart_putc(uart0, *p);
        sleep_ms(1);
        if (uart_is_readable(uart0)) {
            char c = uart_getc(uart0);
            printf("TX=%c RX=%c %s\n", *p, c,
                   c == *p ? "OK" : "MISMATCH");
        } else {
            printf("No RX\n");
        }
    }
}
