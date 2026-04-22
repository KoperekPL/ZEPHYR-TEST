#include "pwm_thread.h"

void pwm_out(void *p1, void *p2, void *p3) {
    // inicjalizacja pwm
    static const struct pwm_dt_spec pwm_l = PWM_DT_SPEC_GET(DT_ALIAS(pwmleft));
    static const struct pwm_dt_spec pwm_r = PWM_DT_SPEC_GET(DT_ALIAS(pwmright));

    while (!pwm_is_ready_dt(&pwm_r) || !pwm_is_ready_dt(&pwm_l)) {
        k_msleep(10); // oczekiwanie na gotowość PWM
    }

    // zmienne wątku
    enum pwm_state state = IDLE; // wartosc poczatkowa stanu wyjsc
    float u; // sterowanie pwm
    uint32_t dead_time_start;

    while (1) {

        k_msgq_get(&pwm_queue, &u, K_NO_WAIT);
        
        // sterowanie sekwencyjne
        if (u > 0) { // sterowanie lewo
            if (state == IDLE) {
                state = LEFT;
            } else if (state == RIGHT) {
                dead_time_start = k_uptime_get_32();
                state = DEAD_TIME_WAIT;
            } else if (state == DEAD_TIME_WAIT) {
                if (k_uptime_get_32() - dead_time_start > DEAD_TIME) {
                    state = LEFT;
                }
            }
        } else if (u < 0) { // sterowanie prawo
            if (state == IDLE) {
                state = RIGHT;
            } else if (state == LEFT) {
                dead_time_start = k_uptime_get_32();
                state = DEAD_TIME_WAIT;
            } else if (state == DEAD_TIME_WAIT) {
                if (k_uptime_get_32() - dead_time_start > DEAD_TIME) {
                    state = RIGHT;
                }
            }
        } else { // zatrzymanie
            if (state != IDLE && state != DEAD_TIME_WAIT) {
                dead_time_start = k_uptime_get_32();
                state = DEAD_TIME_WAIT;
            } else {
                if (k_uptime_get_32() - dead_time_start > DEAD_TIME) {
                    state = IDLE;
                }
            }
        }

        // wyjscia
        float abs_u = fabs(u); // wartosc bezwzgledna
        uint32_t pulse_l = 0;
        uint32_t pulse_r = 0;

        if (state == LEFT) {
            pulse_l = (pwm_l.period * (uint32_t)abs_u) / 100;
        }

        if (state == RIGHT) {
            pulse_r = (pwm_r.period * (uint32_t)abs_u) / 100;
        }

        printk("lewy: %u,    prawy: %u\n", pulse_l, pulse_r);
        pwm_set_pulse_dt(&pwm_l, pulse_l);
        pwm_set_pulse_dt(&pwm_r, pulse_r);

        k_msleep(1000/PWM_FREQ);

    }
}