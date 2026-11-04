// Exchange typed USB lines over UART0; flash the same program on both boards.
// Cross GP0 (TX) to the other board's GP1 (RX), and connect both GNDs.
// Disconnect both USB cables before wiring.

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

#define BAUD 115200

typedef struct {
    char   data[64];
    size_t len;
} line_rx_t;

static void init_uart(uart_inst_t *u, uint baud, uint tx, uint rx) {
    uart_init(u, baud);
    gpio_set_function(tx, GPIO_FUNC_UART);
    gpio_set_function(rx, GPIO_FUNC_UART);
}

// Complete on LF, ignore CR, and truncate input to 63 bytes.
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

// Poll USB input without waiting; skip empty lines.
// Backspace removes one byte, not a full UTF-8 character.
static bool read_typed_line(line_rx_t *r) {
    int c;

    while ((c = getchar_timeout_us(0)) != PICO_ERROR_TIMEOUT) {
        if (c == '\n' || c == '\r') {
            if (r->len == 0) continue;
            r->data[r->len] = '\0';
            r->len = 0;
            printf("\n");
            return true;
        }
        if (c == '\b' || c == 0x7F) {
            if (r->len > 0) { r->len--; printf("\b \b"); }
            continue;
        }
        if (c >= ' ' && r->len < sizeof(r->data) - 1) {
            r->data[r->len++] = (char)c;
            putchar(c);
        }
    }
    return false;
}

int main(void) {
    stdio_init_all();

    init_uart(uart0, BAUD, 0, 1);

    printf("\n=== 보드 간 메시지 ===\n문장을 치고 Enter\n\n");

    line_rx_t typed = {0};
    line_rx_t rx    = {0};

    while (true) {

        if (read_typed_line(&typed)) {
            char out[sizeof(typed.data) + 2];
            int n = snprintf(out, sizeof(out), "%s\n", typed.data);
            uart_write_blocking(uart0, (const uint8_t *)out, n);
            printf("보냄 : %s\n", typed.data);
        }

        if (read_line(uart0, &rx))
            printf("  받음 : %s\n", rx.data);
    }
}
