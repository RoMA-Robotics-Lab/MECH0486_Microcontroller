// Read the MFRC522 version register over SPI.
// MISO GP4, CS GP5, SCK GP6, MOSI GP7, RST GP22; use 3V3 only.
// The module's SDA pin is SPI CS. Wire with power off.

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

#define PIN_MISO 4
#define PIN_CS   5
#define PIN_SCK  6
#define PIN_MOSI 7
#define PIN_RST  22

static spi_inst_t *const s_spi = spi0;

static void init_spi(void) {
    spi_init(spi0, 1000000);
    spi_set_format(spi0, 8, SPI_CPOL_0,
                   SPI_CPHA_0, SPI_MSB_FIRST);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_init(PIN_CS);
    gpio_put(PIN_CS, 1);
    gpio_set_dir(PIN_CS, GPIO_OUT);
}

static inline void cs_low(void)  { gpio_put(PIN_CS, 0); sleep_us(1); }
static inline void cs_high(void) { sleep_us(1); gpio_put(PIN_CS, 1); }

static uint8_t read_reg(uint8_t reg) {
    uint8_t addr = ((reg << 1) & 0x7E) | 0x80;
    uint8_t val = 0;
    cs_low();
    spi_write_blocking(s_spi, &addr, 1);
    spi_read_blocking(s_spi, 0x00, &val, 1);
    cs_high();
    return val;
}

int main(void) {
    stdio_init_all();
    init_spi();
    gpio_init(PIN_RST); gpio_set_dir(PIN_RST, GPIO_OUT);
    gpio_put(PIN_RST, 1); sleep_ms(50);
    while (true) {
        uint8_t v = read_reg(0x37);
        printf("VersionReg: 0x%02X\n", v);
        sleep_ms(1000);
    }
}
