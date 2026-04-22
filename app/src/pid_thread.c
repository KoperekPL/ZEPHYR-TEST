#include "pid_thread.h"
#include <math.h>

// Mapowanie ADC
static float adc_to_angle(uint16_t adc_value) {
    return ((float)adc_value - ADC_MID) * (2.0f * ANGLE_RANGE) / ADC_MAX;
}

// Saturacja
static float clamp_float(float value, float min_val, float max_val) {
    if (value > max_val) return max_val;
    if (value < min_val) return min_val;
    return value;
}

void pid(void *p1, void *p2, void *p3) {
    // zmienne regulatora PID
    float I_state = 0.0f; // stan integratora
    float prev_e  = 0.0f; // poprzedni uchyb 
    bool  first   = true; // flaga pierwszej iteracji
    uint32_t last_time = 0;

    uint16_t reference_raw = ADC_MID;
    uint16_t actual_raw    = ADC_MID;

    while (1) {

        // wątek czeka tutaj aż analog_read_thread wyśle nową próbkę
        if (k_msgq_get(&analog_queue, &actual_raw, K_FOREVER) != 0) {
            continue;
        }

        // odbiór wartości zadanej z UART (bez blokowania)
        // opróżniamy kolejkę żeby dostać najnowszą wartość
        while (k_msgq_get(&uart_queue, &reference_raw, K_NO_WAIT) == 0) {}

        // konwersja na kąt
        float ref = adc_to_angle(reference_raw);
        float actual = adc_to_angle(actual_raw);

        //printk("Otrzymana wartość actual: %f\n", actual);

        // pierwsza iteracja — tylko inicjalizacja
        if (first) {
            prev_e = ref - actual;
            last_time = k_uptime_get_32();
            first = false;
            continue;
        }

        // obliczenie dt
        uint32_t now = k_uptime_get_32();
        float dt = (now - last_time) / 1000.0f; // s

        // zabezpieczenie: jeśli dt ≈ 0, pomiń iterację
        if (dt < 0.001f) {
            continue;
        }

        // uchyb
        float e = ref - actual;

        // człon P
        float P = KP * e;

        // człon D (pochodna uchybu)
        float D = KP * TD * (e - prev_e) / dt;

        // wyjście nieograniczone
        float v = P + I_state + D;

        // saturacja
        float u = clamp_float(v, -U_MAX, U_MAX);

        // błąd saturacji
        float es = u - v;

        // anti-windup back calculation
        I_state += ((KP / TI) * e + (1.0f / TT) * es) * dt;

        // wysłanie sterowania do wątku PWM
        k_msgq_put(&pwm_queue, &u, K_NO_WAIT);

        prev_e = e;
        last_time = now;
    }
}