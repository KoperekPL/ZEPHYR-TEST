#ifndef PID_THREAD_H
#define PID_THREAD_H

#include <zephyr/kernel.h>
#include "uart_thread.h"
#include "analog_read_thread.h"
#include "pwm_thread.h"

// Parametry wątku
#define PID_STACK_SIZE 2048
#define PID_PRIORITY 3

// Parametry regulatora PID
#define KP   1.0f 
#define TI   0.5f
#define TD   0.05f
#define TT   0.2f // stała czasowa anti-windup back calculation (typowo sqrt(TI*TD))

// Limity wyjścia
#define U_MAX  100.0f

// Mapowanie ADC na kąt
// 12-bit ADC (0–4095), potencjometr:
//   ADC = 0    → −45°
//   ADC = 2048 → 0°
//   ADC = 4095 → +45°
#define ADC_MID     2048
#define ANGLE_RANGE 45.0f
#define ADC_MAX     4095

void pid(void *p1, void *p2, void *p3);

#endif /* PID_THREAD_H */