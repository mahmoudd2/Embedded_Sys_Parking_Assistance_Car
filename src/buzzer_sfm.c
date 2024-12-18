#include "buzzer_sfm.h"

// Initialize the buzzer GPIO pin
void buzzer_init() {
    gpio_init(BUZZER_PIN);         // Initialize the GPIO pin
    gpio_set_dir(BUZZER_PIN, GPIO_OUT); // Set the pin as output
    buzzer_off();                  // Ensure the buzzer is off initially
}

// Turn the buzzer on
void buzzer_on() {
    gpio_put(BUZZER_PIN, 1); // Set the pin high to turn the buzzer on
}

// Turn the buzzer off
void buzzer_off() {
    gpio_put(BUZZER_PIN, 0); // Set the pin low to turn the buzzer off
}

// Make the buzzer beep for a specified duration (in milliseconds)
void buzzer_beep(uint32_t duration_ms) {
    buzzer_on();               // Turn the buzzer on
    sleep_ms(duration_ms);     // Wait for the specified duration
    buzzer_off();              // Turn the buzzer off
}