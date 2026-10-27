// Set fan speed from ADC0 using an L9110S motor driver.
// Potentiometer: OUT GP26, VCC 3V3. Driver: A-1A GP15, A-1B GND.
// Use a suitable motor supply and common GND; do not drive the motor directly.

#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"

#define POT   26
#define MOTOR 15

int main() {
    adc_init();
    adc_gpio_init(POT);
    adc_select_input(0);

    gpio_set_function(MOTOR, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(MOTOR);
    pwm_set_wrap(slice, 65535);
    pwm_set_enabled(slice, true);

    while (true) {
        uint16_t raw = adc_read();
        // Scale the 12-bit ADC value to the 16-bit PWM range.
        pwm_set_gpio_level(MOTOR, raw << 4);
        sleep_ms(10);
    }
}
