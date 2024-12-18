#ifndef BUZZER_SFM_H
#define BUZZER_SFM_H

#include "pico/stdlib.h"

// GPIO Pin for Buzzer Control
#define BUZZER_PIN 25  // Define the GPIO pin connected to the buzzer

// Function Prototypes
void buzzer_init(void);
void buzzer_on(void);
void buzzer_off(void);
void buzzer_beep(uint32_t duration_ms);

#endif // BUZZER_SFM_H