#include "uart_thread.h"

void uart_read_data(void *p1, void *p2, void *p3) {
    // inicjalizacja uart
    const struct device *uart_dev = DEVICE_DT_GET(DT_NODELABEL(uart0)); // definicja uart

    while (!device_is_ready(uart_dev)) {
        k_msleep(10); // oczekiwanie na gotowość urządzenia UART
    }

    // zmienne wątku
    unsigned char high_byte;
    unsigned char low_byte;
    uint16_t received_data;
    bool high_byte_in = false; // flagi żeby nie blokować procesora
    bool low_byte_in = false;

    while (1) {
        // TODO: jakies sprawdzenie danych UART, może Byte startowy czy coś
        if (!high_byte_in) {
            if (!uart_poll_in(uart_dev, &high_byte)) high_byte_in = true;
        } else {
            if (!uart_poll_in(uart_dev, &low_byte)) low_byte_in = true;
        }

        if (high_byte_in && low_byte_in) {
            received_data = (high_byte << 8) | low_byte;
            high_byte_in = false;
            low_byte_in = false;
            k_msgq_put(&uart_queue, &received_data, K_NO_WAIT);
        }

        k_msleep(1000/UART_READ_FREQ); // definicja w .h
    }
}