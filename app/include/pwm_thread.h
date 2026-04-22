#ifndef PWM_THREAD_H
#define PWM_THREAD_H

#include <zephyr/kernel.h>
#include <zephyr/drivers/pwm.h>
#include <math.h>

#define PWM_STACK_SIZE 2048
#define PWM_PRIORITY 2 // wysoki priorytet dla odbioru zadanej pozycji

#define PWM_FREQ 10 // czestotliwosc odczytu wartości analogowej w Hz 

#define DEAD_TIME 500 // czas wyłączenia tranzystorów w ms

extern struct k_msgq pwm_queue;

void pwm_out(void *p1, void *p2, void *p3);

enum pwm_state {
    IDLE,
    LEFT,
    DEAD_TIME_WAIT,
    RIGHT
};

#endif /* PWM_THREAD_H */