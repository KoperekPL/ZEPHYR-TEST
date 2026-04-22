#ifndef ANALOG_READ_THREAD_H
#define ANALOG_READ_THREAD_H

#include <zephyr/kernel.h>
#include <zephyr/drivers/adc.h>

#define ANALOG_READ_STACK_SIZE 1024
#define ANALOG_READ_PRIORITY 1 // sensor - wysoki priorytet dla feedback

#define ANALOG_READ_FREQ 10 // czestotliwosc odczytu wartości analogowej w Hz 

extern struct k_msgq analog_queue;

void analog_read_data(void *p1, void *p2, void *p3);

#endif /* ANALOG_READ_THREAD_H */