#include "sharp_infra_sensor.h"
#include "hardware/adc.h"
#include "math.h"

// ADC Resolution and Voltage Reference
#define ADC_MAX_VALUE 4095.0  // 12-bit ADC
#define ADC_REF_VOLTAGE 3.3   // Voltage reference in volts

// Conversion constants for the GP2Y0A21 sensor
// These values depend on the sensor's datasheet and your specific setup.
// The sensor's output voltage corresponds to distance as a nonlinear function.
#define SENSOR_COEFF_A 27.86  // Example coefficient (adjust based on calibration)
#define SENSOR_EXP_B -1.15    // Example exponent (adjust based on calibration)

// Maximum measurable distance (in cm)
#define MAX_DISTANCE 100.0

// Initialize the sensor
void sharp_ir_sensor_init() {
    // Initialize the ADC hardware
    adc_init();

    // Select the ADC pin connected to the sensor
    adc_gpio_init(BACK_SHARP_IR_SENSOR_PIN);
    adc_select_input(BACK_SHARP_IR_SENSOR_PIN - 26); // Assuming GPIO 26 corresponds to ADC channel 0
}

// Read raw ADC value from the sensor
uint16_t sharp_ir_sensor_read() {
    return adc_read();
}

// Get distance in centimeters
float sharp_ir_sensor_get_distance() {
    // Read the ADC value
    uint16_t raw_adc = sharp_ir_sensor_read();

    // Convert ADC value to voltage
    float voltage = (raw_adc / ADC_MAX_VALUE) * ADC_REF_VOLTAGE;

    // Convert voltage to distance using a calibration curve
    // distance = A * voltage^B
    // Ensure calibration matches your specific sensor and setup
    float distance = SENSOR_COEFF_A * pow(voltage, SENSOR_EXP_B);

    // If distance exceeds maximum threshold, return 100 cm
    if (distance > MAX_DISTANCE) {
        distance = MAX_DISTANCE;
    }

    return distance;
}
