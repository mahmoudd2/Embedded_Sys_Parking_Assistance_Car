#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#include "pico/stdlib.h"

// Define default pins for the first ultrasonic sensor
#ifndef FRONT_ULTRASONIC_TRIGGER_PIN
#define FRONT_ULTRASONIC_TRIGGER_PIN 5  // Trigger pin for sensor 1
#endif

#ifndef FRONT_ULTRASONIC_ECHO_PIN
#define FRONT_ULTRASONIC_ECHO_PIN 21    // Echo pin for sensor 1
#endif

// Define default pins for the second ultrasonic sensor
#ifndef BACK_ULTRASONIC_TRIGGER_PIN
#define BACK_ULTRASONIC_TRIGGER_PIN 20  // Trigger pin for sensor 2
#endif

#ifndef BACK_ULTRASONIC_ECHO_PIN
#define BACK_ULTRASONIC_ECHO_PIN 19    // Echo pin for sensor 2
#endif

// Function to initialize an ultrasonic sensor
void ultrasonic_sensor_init(uint trigger_pin, uint echo_pin);

// Function to measure the pulse duration on a specified pin
uint32_t pulse_in(uint pin, bool state, uint timeout_us);

// Function to read the distance measured by an ultrasonic sensor in centimeters
float ultrasonic_sensor_read_distance(uint trigger_pin, uint echo_pin);

#endif // ULTRASONIC_SENSOR_H
