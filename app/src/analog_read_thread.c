#include "analog_read_thread.h"

void analog_read_data(void *p1, void *p2, void *p3) {
    // inicjalizacja analog in
    static const struct adc_dt_spec pot_channel = ADC_DT_SPEC_GET(DT_PATH(zephyr_user));

    while (!adc_is_ready_dt(&pot_channel)) {
        k_msleep(10); // oczekiwanie na gotowość analog
    }

    adc_channel_setup_dt(&pot_channel);

    // zmienne wątku
    uint16_t analog_value;
    int err_adc;

    struct adc_sequence sequence = {
        .buffer = &analog_value, // gdzie zapisac wynik
        .buffer_size = sizeof(analog_value), // rozmiar bufora
    };

    while (1) {
        // odczyt
        adc_sequence_init_dt(&pot_channel, &sequence);
        sequence.resolution = 12; // RPi Pico ma 12-bitowy przetwornik ADC

        err_adc = adc_read(pot_channel.dev, &sequence);

        if (err_adc < 0) {
            printk("Błąd odczytu ADC: %d\n", err_adc);
        } else {
            //printk("odczytany wynik: %u\n", analog_value);
        }
        
        k_msgq_put(&analog_queue, &analog_value, K_NO_WAIT);

        k_msleep(1000/ANALOG_READ_FREQ); // definicja w .h
    }
}