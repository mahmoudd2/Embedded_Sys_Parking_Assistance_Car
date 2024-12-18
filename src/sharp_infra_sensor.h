#ifndef SHARP_INFRA_SENSOR_H
#define SHARP_INFRA_SENSOR_H

#include <stdint.h>

// Pin connected to the sensor's output (adjust as needed)
#define BACK_SHARP_IR_SENSOR_PIN 27 // ADC pin for reading the sensor

// Function declarations
void sharp_ir_sensor_init();       // Initialize the sensor
uint16_t sharp_ir_sensor_read();   // Read raw ADC value
float sharp_ir_sensor_get_distance(); // Get distance in cm

#endif // SHARP_INFRA_SENSOR_H