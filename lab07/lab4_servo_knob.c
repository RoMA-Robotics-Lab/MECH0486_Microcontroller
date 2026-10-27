// Set servo angle from ADC0.
// Servo signal: GP0. Potentiometer: OUT GP26, VCC 3V3.
// Use a suitable servo supply and a common GND.

#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

#define SERVO 0
#define POT   26

static uint servo_slice;

void servo_init(void) {
    gpio_set_function(SERVO, GPIO_FUNC_PWM);
    servo_slice = pwm_gpio_to_slice_num(SERVO);

    // Set a 1 us PWM tick from the configured system clock.
    pwm_set_clkdiv(servo_slice, clock_get_hz(clk_sys) / 1000000.0f);

    pwm_set_wrap(servo_slice, 19999);
    pwm_set_enabled(servo_slice, true);
}

// Map 0-180 degrees to 1000-2000 us pulses.
void servo_angle(int deg) {
    if (deg < 0)   deg = 0;
    if (deg > 180) deg = 180;
    int us = 1000 + (deg * 1000) / 180;
    pwm_set_gpio_level(SERVO, us);
}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main() {
    adc_init();
    adc_gpio_init(POT);
    adc_select_input(0);
    servo_init();

    while (true) {
        int deg = map(adc_read(), 0, 4095, 0, 180);
        servo_angle(deg);
        sleep_ms(20);
    }
}
