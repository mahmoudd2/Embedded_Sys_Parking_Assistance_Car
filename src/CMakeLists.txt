# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.12)

# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Set name of project (as PROJECT_NAME) and C/C++ standards
project(blink C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

# Tell CMake where to find the executable source files
add_executable(${PROJECT_NAME}
    main.c
    servo_control.c
    ultraSonic_sensor.c
    infrared_sensor.c
    dc_motor.c
    sharp_infra_sensor.c
    buzzer_sfm.c
)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

# Link to pico_stdlib (gpio, time, etc. functions) and hardware libraries
target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    hardware_pwm
    hardware_adc
    FreeRTOS-Kernel-Heap4  # Assuming you still need this for FreeRTOS support
)

# Enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# Set the include directories for the project (including current directory)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)
