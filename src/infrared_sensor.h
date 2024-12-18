#ifndef INFRARED_SENSOR_H
#define INFRARED_SENSOR_H
#include "pico/stdlib.h"

// #include <stdint.h>

// Define the default pin for the infrared sensor
#ifndef FRONT_IR_PIN
#define FRONT_IR_PIN 15  // Default GPIO pin for infrared sensor
#endif
#define LED_PIN         25  // LED (GPIO 25)

// Function to initialize the infrared sensor
void infrared_sensor_init(uint sensor_pin);
// Function to read the state of the infrared sensor
bool infrared_sensor_is_triggered(uint sensor_pin);

#define BIT_SET(reg, pin)    (*(volatile uint32_t *)(reg) |= (1 << (pin)))
#define BIT_CLEAR(reg, pin)  (*(volatile uint32_t *)(reg) &= ~(1 << (pin)))
#define BIT_TOGGLE(reg, pin) (*(volatile uint32_t *)(reg) ^= (1 << (pin)))
#define BIT_READ(reg, pin)   (*(volatile uint32_t *)(reg) & (1 << (pin)))

// void Ultrasonic_init();
// void Ultrasonic_on();
// void Ultrasonic_off();
// float Ultrasonic_readDistance();
// void Ultrasonic_calibrate();

#endif // SENSORS_H