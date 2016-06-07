#include <string.h>

#include "aes.h"
#include "block.h"

static uint8_t g_counter[16];

void aes128_ctr_init(uint8_t *key, uint8_t *counter) {
    if (counter != NULL) {
        memcpy(g_counter, counter, 16);
    }
    aes128_init(key);
}

void aes128_ctr_evolve_counter(void) {
    block_incr(g_counter);
    return;
}

void aes128_ctr(uint8_t *dest, uint8_t *msg, uint32_t msg_len) {
    uint8_t buffer[16];
    uint32_t num_blocks = msg_len / 16;
    for (uint32_t i = 0; i < num_blocks; i++) {
        aes128_ecb(buffer, g_counter);
        aes128_ctr_evolve_counter();
        block_xor(dest + (i * 16), msg + (i * 16), buffer);
    }
    if (msg_len % 16) {
        aes128_ecb(buffer, g_counter);
        for (uint8_t i = 0; i < msg_len % 16; i++) {
            dest[num_blocks * 16 + i] = msg[num_blocks * 16 + i] ^ buffer[i];
        }
    }
}
