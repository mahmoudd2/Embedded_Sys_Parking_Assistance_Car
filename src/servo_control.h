#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H

#include "pico/stdlib.h"

// GPIO Pins for Servo Control
#define FRONT_SERVO_PWM_PIN 26  // First servo PWM pin
#define BACK_SERVO_PWM_PIN 12  // Second servo PWM pin

// Servo PWM Settings   
#define SERVO_MIN_PULSE_WIDTH 500   // Minimum pulse width in microseconds for 0°
#define SERVO_MAX_PULSE_WIDTH 2500  // Maximum pulse width in microseconds for 180°
#define SERVO_FREQUENCY 50          // Servo frequency for SG90 (50 Hz)

void servo_init(void);
void servo_set_position(uint gpio_pin, float angle);

#endif // SERVO_CONTROL_H
