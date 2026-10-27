// Set LED brightness from ADC0.
// Potentiometer: OUT GP26, VCC-VSYS. LED: GP25 or GP15 with a series resistor.

#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"

#define POT 26
#define LED 25

int main() {
    adc_init();
    adc_gpio_init(POT);
    adc_select_input(0);

    gpio_set_function(LED, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(LED);
    pwm_set_wrap(slice, 65535);
    pwm_set_enabled(slice, true);

    while (true) {
        uint16_t raw = adc_read();
        // Scale the 12-bit ADC value to the 16-bit PWM range.
        pwm_set_gpio_level(LED, raw << 4);
        sleep_ms(10);
    }
}
