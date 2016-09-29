#include <stdint.h>

#include "nrf.h"

uint8_t rng_byte(void) {
    NRF_RNG->TASKS_START = 1;
    while (!NRF_RNG->EVENTS_VALRDY) {
        /* Waiting for hardware */
    }
    uint8_t val = (uint8_t)NRF_RNG->VALUE;
    NRF_RNG->TASKS_STOP = 1;
    NRF_RNG->EVENTS_VALRDY = 0;
    return val;
}

void rng_bytes(uint8_t *out, uint32_t num_bytes) {
    NRF_RNG->TASKS_START = 1;
    for (uint32_t i = 0; i < num_bytes; i++) {
        while (!NRF_RNG->EVENTS_VALRDY) {
            ;
        }
        out[i] = (uint8_t)NRF_RNG->VALUE;
        NRF_RNG->EVENTS_VALRDY = 0;
    }
    NRF_RNG->TASKS_STOP = 1;
    return;
}
