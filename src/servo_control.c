#include "servo_control.h"
#include "hardware/pwm.h"

// Function to initialize PWM for servo control
void servo_init() {
    // Initialize both PWM pins
    gpio_set_function(FRONT_SERVO_PWM_PIN, GPIO_FUNC_PWM);
    gpio_set_function(BACK_SERVO_PWM_PIN, GPIO_FUNC_PWM);

    // Get PWM slice numbers
    uint slice_num1 = pwm_gpio_to_slice_num(FRONT_SERVO_PWM_PIN);
    uint slice_num2 = pwm_gpio_to_slice_num(BACK_SERVO_PWM_PIN);

    // Configure PWM for 50 Hz (20 ms period)
    pwm_set_wrap(slice_num1, 20000);
    pwm_set_wrap(slice_num2, 20000);

    // Set clock divider for a 20 ms period with 125 MHz clock
    pwm_set_clkdiv(slice_num1, 125.0);
    pwm_set_clkdiv(slice_num2, 125.0);

    // Enable PWM output
    pwm_set_enabled(slice_num1, true);
    pwm_set_enabled(slice_num2, true);
}

// Function to set servo position based on angle (-180° to 180°)
void servo_set_position(uint gpio_pin, float angle) {
    // Clamp the angle between -180 and 180
    if (angle < -180) angle = -180;
    if (angle > 180) angle = 180;

    // Adjust the angle to map it to the correct PWM pulse width range
    uint16_t pulse_width;

    if (angle < 0) {
        pulse_width = SERVO_MIN_PULSE_WIDTH + ((angle + 180) / 180.0) * (SERVO_MAX_PULSE_WIDTH - SERVO_MIN_PULSE_WIDTH);
    } else {
        pulse_width = SERVO_MIN_PULSE_WIDTH + (angle / 180.0) * (SERVO_MAX_PULSE_WIDTH - SERVO_MIN_PULSE_WIDTH);
    }

    // Set the PWM duty cycle to the calculated pulse width
    uint slice_num = pwm_gpio_to_slice_num(gpio_pin);
    pwm_set_gpio_level(gpio_pin, pulse_width);
}
