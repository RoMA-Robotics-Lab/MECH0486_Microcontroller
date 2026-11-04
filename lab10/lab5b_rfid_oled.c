// Display RFID card UIDs on an SSD1306 OLED.
// OLED: SDA GP0, SCL GP1. RC522: MISO GP4, CS GP5, SCK GP6, MOSI GP7, RST GP22.
// Both modules use 3V3 and common GND.

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/spi.h"
#include "ssd1306.h"
#include "mfrc522.h"

#define SPI_PORT   spi0
#define PIN_MISO   4
#define PIN_CS     5
#define PIN_SCK    6
#define PIN_MOSI   7
#define PIN_RST    22

#define OLED_ADDR  0x3C
#define POLL_MS    20
#define READ_TRIES  5
#define GONE_COUNT  3

static const struct {
    uint8_t     uid[4];
    const char *name;
} registered[] = {
    { { 0xCA, 0xD1, 0x5F, 0xF8 }, "CARD 1" },
};

static int find_registered(const uint8_t uid[4]) {
    for (int i = 0; i < (int)(sizeof(registered) / sizeof(registered[0])); i++)
        if (memcmp(uid, registered[i].uid, 4) == 0) return i;
    return -1;
}

static bool ask_card(uint8_t uid[4]) {
    for (int i = 0; i < READ_TRIES; i++)
        if (mfrc522_read_uid(uid)) return true;
    return false;
}

static void draw(const char *l1, const char *l2, const char *l3) {
    ssd1306_clear();
    ssd1306_text(0, 0, l1);
    ssd1306_text(0, 2, l2);
    ssd1306_text(0, 4, l3);
    ssd1306_show();
}

int main(void) {
    stdio_init_all();
    sleep_ms(2000);

    i2c_init(i2c0, 100000);
    gpio_set_function(0, GPIO_FUNC_I2C);
    gpio_set_function(1, GPIO_FUNC_I2C);
    gpio_pull_up(0); gpio_pull_up(1);
    ssd1306_init(i2c0, OLED_ADDR);

    spi_init(SPI_PORT, 1000 * 1000);
    gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    mfrc522_init(SPI_PORT, PIN_CS, PIN_RST);

    printf("RFID -> OLED  (MFRC522 0x%02X)\n", mfrc522_version());
    draw("RFID READER", "NO CARD", "");

    uint8_t  uid[4], last[4] = { 0 };
    bool     present = false;
    unsigned miss = 0, n_detect = 0;

    while (true) {
        if (ask_card(uid)) {
            miss = 0;

            if (!present || memcmp(uid, last, 4) != 0) {
                present = true;
                memcpy(last, uid, 4);
                n_detect++;

                char line[SSD1306_COLS + 1];
                snprintf(line, sizeof(line), "%02X %02X %02X %02X",
                         uid[0], uid[1], uid[2], uid[3]);

                int idx = find_registered(uid);
                draw("CARD FOUND", line,
                     idx >= 0 ? registered[idx].name : "NOT REGISTERED");

                printf("[%u] %s -> %s\n", n_detect, line,
                       idx >= 0 ? registered[idx].name : "NOT REGISTERED");
            }
        } else if (present && ++miss >= GONE_COUNT) {
            present = false;
            miss    = 0;
            draw("RFID READER", "NO CARD", "");
            printf("카드 치움 (감지 %u)\n", n_detect);
        }

        sleep_ms(POLL_MS);
    }
}
