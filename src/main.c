#include <stdio.h>
#include "pico/stdlib.h"
#include "servo_control.h"
#include "infrared_sensor.h"
#include "ultrasonic_sensor.h"
#include "dc_motor.h"
#include "sharp_infra_sensor.h"
#include "buzzer_sfm.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

// Thresholds
#define OBSTACLE_DISTANCE 5.0       // cm
#define PARKING_SPOT_MIN_DISTANCE 20.0 // cm
#define PARKING_SPOT_MAX_DISTANCE 40.0 // cm
#define PARKING_ROTATE_DURATION 2000   // ms for rotating during parking
#define MOVING_SPEED -0.3           // Default moving speed
#define MINIMUM_MOVEMENT_DURATION 3000 // ms to move before parking check

// Global Variables
volatile bool stopCondition = false;  // Shared flag to indicate stop condition
volatile bool parkingMode = false;    // Flag to indicate parking mode
volatile bool completed = false;      // Flag to indicate parking is complete and all tasks should stop
SemaphoreHandle_t stopConditionSemaphore;

// Task Prototypes
void initialize_components();
void task_movement_controller(void *pvParameters);
void task_parking_mechanism(void *pvParameters);

// Main Function
int main() {
    // Initialize all components
    initialize_components();

    // Create a semaphore for accessing the stop condition
    stopConditionSemaphore = xSemaphoreCreateMutex();

    // Create FreeRTOS tasks
    xTaskCreate(task_movement_controller, "MovementController", 256, NULL, 2, NULL);
    xTaskCreate(task_parking_mechanism, "ParkingMechanism", 256, NULL, 1, NULL);

    // Start FreeRTOS scheduler
    vTaskStartScheduler();

    while (1) {
        // This will not run once FreeRTOS scheduler starts
    }
}

// Component Initialization
void initialize_components() {
    stdio_init_all();
    buzzer_init();
    dc_motor_init();
    servo_init();
    infrared_sensor_init(FRONT_IR_PIN);
    sharp_ir_sensor_init();
    ultrasonic_sensor_init(FRONT_ULTRASONIC_TRIGGER_PIN, FRONT_ULTRASONIC_ECHO_PIN);
    ultrasonic_sensor_init(BACK_ULTRASONIC_TRIGGER_PIN, BACK_ULTRASONIC_ECHO_PIN);

    servo_set_position(FRONT_SERVO_PWM_PIN, 0);  // Front ultrasonic facing forward
    servo_set_position(BACK_SERVO_PWM_PIN, 90);  // Back ultrasonic facing right
    printf("Components initialized.\n");
}

// Task: Movement Controller
void task_movement_controller(void *pvParameters) {
    bool initialMovementComplete = false;

    while (1) {
        // Check if parking is completed
        if (completed) {
            printf("[Movement Controller] Parking complete. Stopping task.\n");
            vTaskDelete(NULL);  // Terminate this task
        }

        // Read sensor values
        float front_distance = ultrasonic_sensor_read_distance(FRONT_ULTRASONIC_TRIGGER_PIN, FRONT_ULTRASONIC_ECHO_PIN);
        float back_distance = ultrasonic_sensor_read_distance(BACK_ULTRASONIC_TRIGGER_PIN, BACK_ULTRASONIC_ECHO_PIN);
        bool front_IR = infrared_sensor_is_triggered(FRONT_IR_PIN);

        printf("[Movement Controller] Front Distance: %.2f cm, Back Distance: %.2f cm, IR Triggered: %d\n", front_distance, back_distance, !front_IR);

        xSemaphoreTake(stopConditionSemaphore, portMAX_DELAY);

        // Check for obstacles
        if (front_distance <= OBSTACLE_DISTANCE || !front_IR || back_distance <= OBSTACLE_DISTANCE) {
            stopCondition = true;
            parkingMode = false;  // Ensure parking mode is disabled
            dc_motor_stop("both");
            buzzer_beep(500);  // Beep to indicate stop
            printf("[Movement Controller] Stop condition met. Motors stopped.\n");
        } else {
            stopCondition = false;
            if (!parkingMode) {
                if (!initialMovementComplete) {
                    printf("[Movement Controller] Moving for at least 3 seconds before checking for parking spots.\n");
                    dc_motor_set_speed("both", MOVING_SPEED);
                    vTaskDelay(MINIMUM_MOVEMENT_DURATION / portTICK_PERIOD_MS);
                    initialMovementComplete = true;
                }

                // Check for parking spot
                if (back_distance >= PARKING_SPOT_MIN_DISTANCE && back_distance <= PARKING_SPOT_MAX_DISTANCE) {
                    printf("[Movement Controller] Parking spot found. Entering parking mode.\n");
                    parkingMode = true;  // Enable parking mode
                } else {
                    // Keep moving if no parking spot is detected
                    dc_motor_set_speed("both", MOVING_SPEED);
                    printf("[Movement Controller] Moving at speed %.2f.\n", MOVING_SPEED);
                }
            }
        }
        xSemaphoreGive(stopConditionSemaphore);

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

// Task: Parking Mechanism
void task_parking_mechanism(void *pvParameters) {
    while (1) {
        // Check if parking is completed
        if (completed) {
            printf("[Parking Mechanism] Parking complete. Stopping task.\n");
            vTaskDelete(NULL);  // Terminate this task
        }

        xSemaphoreTake(stopConditionSemaphore, portMAX_DELAY);

        if (parkingMode && !stopCondition) {
            printf("[Parking Mechanism] Starting parking procedure.\n");

            // Rotate car to the right
            dc_motor_set_speed("left", -0.5); // Left motor moves faster in reverse
            dc_motor_set_speed("right", -0.1); // Right motor moves slower in reverse
            vTaskDelay(PARKING_ROTATE_DURATION / portTICK_PERIOD_MS);

            // Stop motors after parking
            dc_motor_stop("both");
            parkingMode = false;  // Disable parking mode
            completed = true;     // Indicate parking is complete
            printf("[Parking Mechanism] Parking complete. Motors stopped.\n");
        }
        xSemaphoreGive(stopConditionSemaphore);

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
