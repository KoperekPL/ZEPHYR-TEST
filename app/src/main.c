#include <zephyr/kernel.h>
#include <zephyr/drivers/uart.h>

#include "uart_thread.h"
#include "analog_read_thread.h"
#include "pid_thread.h"
#include "pwm_thread.h"

// INIT
// inicjalizacja kolejek
K_MSGQ_DEFINE(uart_queue, sizeof(uint16_t), 10, 2);
K_MSGQ_DEFINE(analog_queue, sizeof(uint16_t), 10, 2);
K_MSGQ_DEFINE(pwm_queue, sizeof(float), 4, 4); // PID → PWM thread

// inicjalizacja wątków
K_THREAD_DEFINE(uart_com, UART_STACK_SIZE, uart_read_data, NULL, NULL, NULL, UART_PRIORITY, 0, 0);
K_THREAD_DEFINE(analog_read, ANALOG_READ_STACK_SIZE, analog_read_data, NULL, NULL, NULL, ANALOG_READ_PRIORITY, 0, 0);
K_THREAD_DEFINE(pid_thread, PID_STACK_SIZE, pid, NULL, NULL, NULL, PID_PRIORITY, 0, 0);
K_THREAD_DEFINE(pwm_thread, PWM_STACK_SIZE, pwm_out, NULL, NULL, NULL, PWM_PRIORITY, 0, 0);

int main(void) {

  return 0;
}