// Fade an LED using hardware PWM.
// LED: GP2 through a series resistor to GND.

#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define LED 25

int main() {
    gpio_set_function(LED, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(LED);
    pwm_set_wrap(slice, 65535);
    pwm_set_enabled(slice, true);

    int level = 0, step = 512;
    while (true) {
        pwm_set_gpio_level(LED, level);
        level += step;
        if (level <= 0 || level >= 65535) step = -step;
        sleep_ms(20);
    }
}
