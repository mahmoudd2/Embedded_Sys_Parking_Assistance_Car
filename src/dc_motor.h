#ifndef DC_MOTOR_H
#define DC_MOTOR_H

#include "pico/stdlib.h"

// GPIO Pins for Motor Control
#define LEFT_MOTOR_IN1_PIN 16   // IN1 pin connected to motor driver
#define LEFT_MOTOR_IN2_PIN 17   // IN2 pin connected to motor driver
#define RIGHT_MOTOR_IN3_PIN 28   // IN2 pin connected to motor driver
#define RIGHT_MOTOR_IN4_PIN 29   // IN2 pin connected to motor driver

// Motor PWM Settings
#define MOTOR_FREQUENCY 1000  // Motor frequency (1 kHz)

// Function Prototypes
void dc_motor_init(void);
void dc_motor_set_speed(const char *side, float speed);
void dc_motor_stop(const char *side);

#endif // DC_MOTOR_H
