// Send one line per second from UART0 to UART1 on the same board.
// Connect GP0 (TX0) to GP5 (RX1); remove the GP0-GP1 loopback jumper.
// Disconnect power before rewiring. Logs use USB CDC.

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

#define BAUD_TX 115200
#define BAUD_RX 115200

typedef struct {
    char   data[64];
    size_t len;
} line_rx_t;

static void init_uart(uart_inst_t *u, uint baud, uint tx, uint rx) {
    uart_init(u, baud);
    gpio_set_function(tx, GPIO_FUNC_UART);
    gpio_set_function(rx, GPIO_FUNC_UART);
}

// Complete on LF, ignore CR, and truncate lines to 63 characters.
static bool read_line(uart_inst_t *u, line_rx_t *r) {
    while (uart_is_readable(u)) {
        char c = uart_getc(u);

        if (c == '\n') {
            r->data[r->len] = '\0';
            r->len = 0;
            return true;
        }
        if (c != '\r' && r->len < sizeof(r->data) - 1)
            r->data[r->len++] = c;
    }
    return false;
}

int main(void) {
    stdio_init_all();

    init_uart(uart0, BAUD_TX, 0, 1);
    init_uart(uart1, BAUD_RX, 4, 5);

    printf("\n=== uart0 -> uart1 ===\n\n");

    line_rx_t rx = {0};
    uint32_t  seq = 0;
    uint64_t  next = time_us_64() + 1000000;

    while (true) {

        if (time_us_64() >= next) {
            next += 1000000;  // Keep one-second deadlines.

            char out[32];
            int n = snprintf(out, sizeof(out), "MSG %lu\n", (unsigned long)seq++);
            uart_write_blocking(uart0, (const uint8_t *)out, n);
            printf("보냄 : MSG %lu\n", (unsigned long)(seq - 1));
        }

        if (read_line(uart1, &rx))
            printf("  받음 : %s\n", rx.data);
    }
}
