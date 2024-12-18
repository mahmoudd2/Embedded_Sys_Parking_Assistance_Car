#include "infrared_sensor.h"

void infrared_sensor_init(uint sensor_pin) {
    gpio_init(sensor_pin);
    gpio_set_dir(sensor_pin, GPIO_IN);
}
// Check if the infrared sensor is triggered
bool infrared_sensor_is_triggered(uint sensor_pin) {
    return gpio_get(sensor_pin);
}