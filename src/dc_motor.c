#include "dc_motor.h"
#include "hardware/pwm.h"

// Global variable to store PWM slice number
static uint slice_num_in1;
static uint slice_num_in2;
static uint slice_num_in3;
static uint slice_num_in4;

// Initialize the DC motor
void dc_motor_init() {
    // Set IN1 and IN2 as PWM-capable GPIOs
    gpio_set_function(LEFT_MOTOR_IN1_PIN, GPIO_FUNC_PWM);
    gpio_set_function(LEFT_MOTOR_IN2_PIN, GPIO_FUNC_PWM);
    gpio_set_function(RIGHT_MOTOR_IN3_PIN, GPIO_FUNC_PWM);
    gpio_set_function(RIGHT_MOTOR_IN4_PIN, GPIO_FUNC_PWM);
    // Get PWM slice numbers
    slice_num_in1 = pwm_gpio_to_slice_num(LEFT_MOTOR_IN1_PIN);
    slice_num_in2 = pwm_gpio_to_slice_num(LEFT_MOTOR_IN2_PIN);
    slice_num_in3 = pwm_gpio_to_slice_num(RIGHT_MOTOR_IN3_PIN);
    slice_num_in4 = pwm_gpio_to_slice_num(RIGHT_MOTOR_IN4_PIN);
    // Configure PWM frequency
    uint32_t clock_rate = 125000000; // Default clock rate for Raspberry Pi Pico
    uint32_t wrap_value = clock_rate / MOTOR_FREQUENCY - 1;

    //LEFT
    pwm_set_wrap(slice_num_in1, wrap_value);
    pwm_set_wrap(slice_num_in2, wrap_value);
    pwm_set_clkdiv(slice_num_in1, 1.0);
    pwm_set_clkdiv(slice_num_in2, 1.0);

    //RIGHT
    pwm_set_wrap(slice_num_in3, wrap_value);
    pwm_set_wrap(slice_num_in4, wrap_value);
    pwm_set_clkdiv(slice_num_in3, 1.0);
    pwm_set_clkdiv(slice_num_in4, 1.0);

    // Enable PWM output
    pwm_set_enabled(slice_num_in1, true);
    pwm_set_enabled(slice_num_in2, true);
    pwm_set_enabled(slice_num_in3, true);
    pwm_set_enabled(slice_num_in4, true);

    // Set initial motor state to stopped
    pwm_set_gpio_level(LEFT_MOTOR_IN1_PIN, 0);
    pwm_set_gpio_level(LEFT_MOTOR_IN2_PIN, 0);

    pwm_set_gpio_level(RIGHT_MOTOR_IN3_PIN, 0);
    pwm_set_gpio_level(RIGHT_MOTOR_IN4_PIN, 0);
}

// Set motor speed and direction
// Speed range: -1.0 to 1.0 (negative for reverse, positive for forward)
// Set motor speed and direction for a specific side
// side: "right" or "left"
// speed range: -1.0 to 1.0 (negative for reverse, positive for forward)
// Set motor speed and direction for a specific side or both
// side: "right", "left", or "both"
// speed range: -1.0 to 1.0 (negative for reverse, positive for forward)
void dc_motor_set_speed(const char *side, float speed) {
    // Clamp speed to -1.0 to 1.0
    if (speed < -1.0) speed = -1.0;
    if (speed > 1.0) speed = 1.0;

    if (strcmp(side, "left") == 0 || strcmp(side, "both") == 0) {
        // LEFT WHEELS
        if (speed > 0) {
            // Forward direction
            pwm_set_gpio_level(LEFT_MOTOR_IN1_PIN, speed * 65535); // PWM level for IN1
            pwm_set_gpio_level(LEFT_MOTOR_IN2_PIN, 0);             // IN2 off
        } else if (speed < 0) {
            // Reverse direction
            pwm_set_gpio_level(LEFT_MOTOR_IN1_PIN, 0);             // IN1 off
            pwm_set_gpio_level(LEFT_MOTOR_IN2_PIN, -speed * 65535);// PWM level for IN2
        } else {
            // Stop left motors
            dc_motor_stop("left");
        }
    }

    if (strcmp(side, "right") == 0 || strcmp(side, "both") == 0) {
        // RIGHT WHEELS
        if (speed > 0) {
            // Forward direction
            pwm_set_gpio_level(RIGHT_MOTOR_IN3_PIN, speed * 65535); // PWM level for IN3
            pwm_set_gpio_level(RIGHT_MOTOR_IN4_PIN, 0);             // IN4 off
        } else if (speed < 0) {
            // Reverse direction
            pwm_set_gpio_level(RIGHT_MOTOR_IN3_PIN, 0);             // IN3 off
            pwm_set_gpio_level(RIGHT_MOTOR_IN4_PIN, -speed * 65535);// PWM level for IN4
        } else {
            // Stop right motors
            dc_motor_stop("right");
        }
    }
}



// Stop the motor for a specific side
// side: "right", "left", or "both"
void dc_motor_stop(const char *side) {
    if (strcmp(side, "left") == 0) {
        // Stop the left motors (IN1 and IN2)
        pwm_set_gpio_level(LEFT_MOTOR_IN1_PIN, 0);
        pwm_set_gpio_level(LEFT_MOTOR_IN2_PIN, 0);
    } else if (strcmp(side, "right") == 0) {
        // Stop the right motors (IN3 and IN4)
        pwm_set_gpio_level(RIGHT_MOTOR_IN3_PIN, 0);
        pwm_set_gpio_level(RIGHT_MOTOR_IN4_PIN, 0);
    } else if (strcmp(side, "both") == 0) {
        // Stop both motors
        pwm_set_gpio_level(LEFT_MOTOR_IN1_PIN, 0);
        pwm_set_gpio_level(LEFT_MOTOR_IN2_PIN, 0);
        pwm_set_gpio_level(RIGHT_MOTOR_IN3_PIN, 0);
        pwm_set_gpio_level(RIGHT_MOTOR_IN4_PIN, 0);
    }
}

