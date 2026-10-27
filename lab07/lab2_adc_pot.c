// Read ADC0 and compare a single sample with a 16-sample average.
// Potentiometer: OUT GP26, VCC 3V3, GND common.

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

#define POT 26

int main() {
    stdio_init_all();
    adc_init();
    adc_gpio_init(POT);
    adc_select_input(0);

    while (true) {
        uint16_t raw = adc_read();
        float v = raw * 3.3f / 4096;

        uint32_t sum = 0;
        for (int i = 0; i < 16; i++) sum += adc_read();
        uint16_t avg = sum / 16;

        printf("raw=%4u (%.2fV)   평균16=%4u (%.2fV)\n",
               raw, v, avg, avg * 3.3f / 4096);
        sleep_ms(200);
    }
}
