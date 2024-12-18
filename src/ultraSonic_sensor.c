#include "ultrasonic_sensor.h"
#include <stdio.h>

// Initialize the ultrasonic sensor pins
void ultrasonic_sensor_init(uint trigger_pin, uint echo_pin) {
    gpio_init(trigger_pin);
    gpio_set_dir(trigger_pin, GPIO_OUT);  // Set trigger pin as output

    gpio_init(echo_pin);
    gpio_set_dir(echo_pin, GPIO_IN);      // Set echo pin as input
}

// Function to measure the pulse duration on a specified pin
uint32_t pulse_in(uint pin, bool state, uint timeout_us) {
    uint32_t duration = 0;
    absolute_time_t start_time = get_absolute_time();

    // Wait for any previous pulse to end
    while (gpio_get(pin) == state) {
        if (absolute_time_diff_us(start_time, get_absolute_time()) > timeout_us) {
            return 0; // Timeout, return 0
        }
    }

    // Wait for the pulse to start
    while (gpio_get(pin) != state) {
        if (absolute_time_diff_us(start_time, get_absolute_time()) > timeout_us) {
            return 0; // Timeout, return 0
        }
    }

    // Measure the time the pulse is in the desired state
    start_time = get_absolute_time();
    while (gpio_get(pin) == state) {
        if (absolute_time_diff_us(start_time, get_absolute_time()) > timeout_us) {
            return 0; // Timeout, return 0
        }
    }
    absolute_time_t end_time = get_absolute_time();

    // Calculate pulse duration in microseconds
    duration = absolute_time_diff_us(start_time, end_time);
    return duration;
}

// Function to read the distance in centimeters
float ultrasonic_sensor_read_distance(uint trigger_pin, uint echo_pin) {
    // Send a 10us pulse to trigger the sensor
    gpio_put(trigger_pin, 0);
    sleep_us(2);
    gpio_put(trigger_pin, 1);
    sleep_us(10);
    gpio_put(trigger_pin, 0);

    // Measure the echo pulse duration (in microseconds)
    uint32_t duration = pulse_in(echo_pin, 1, 10000);  // 10ms timeout for example

    // If no object is detected (pulse duration too long), return 100 cm
    if (duration == 0 || duration > 30000) {  // Assuming a duration greater than 30000 µs means max range
        return 100.0f;
    }

    // Calculate the distance in centimeters (speed of sound is ~343 m/s or 0.0343 cm/µs)
    float distance = (duration * 0.0343f) / 2.0f;

    return distance;
}
