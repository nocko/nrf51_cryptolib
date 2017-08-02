#include <string.h>

#include "aes.h"
#include "block.h"

static uint8_t g_counter[16];

void aes128_ctr_init(uint8_t *key, uint8_t *counter) {
    if (counter != NULL) {
        memcpy(g_counter, counter, 16);
    }
    if (key != NULL) {
        aes128_init(key);
    }
}

static void aes128_ctr_evolve_counter(void) {
    block_incr(g_counter);
    return;
}

void aes128_ctr(uint8_t *dest, uint8_t *msg, uint32_t msg_len) {
    uint8_t *buffer;
    uint32_t num_blocks = msg_len >> 4;
    for (uint32_t i = 0; i < num_blocks; i++) {
        buffer = aes128_ecb(g_counter);
        aes128_ctr_evolve_counter();
        block_xor(dest + (i << 4), msg + (i << 4), buffer);
    }
    if (msg_len & 15) {
        buffer = aes128_ecb(g_counter);
        for (uint8_t i = 0; i < (msg_len & 15); i++) {
            dest[(num_blocks << 4) + i] = msg[(num_blocks << 4) + i] ^ buffer[i];
        }
    }
}
