cmake_minimum_required(VERSION 3.13.1)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(app LANGUAGES C)

target_sources(app PRIVATE 
    src/main.c
    src/analog_read_thread.c
    src/uart_thread.c
    src/pid_thread.c
    src/pwm_thread.c
)

target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
