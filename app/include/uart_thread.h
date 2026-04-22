#ifndef UART_THREAD_H
#define UART_THREAD_H

#include <zephyr/kernel.h>
#include <zephyr/drivers/uart.h>

#define UART_STACK_SIZE 1024
#define UART_PRIORITY 2 // wysoki priorytet dla odbioru zadanej pozycji

#define UART_READ_FREQ 10 // czestotliwosc odczytu wartości analogowej w Hz 

extern struct k_msgq uart_queue;

void uart_read_data(void *p1, void *p2, void *p3);

#endif /* UART_THREAD_H */