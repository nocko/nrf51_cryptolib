#include <string.h>

#include "aes.h"
#include "block.h"

static uint8_t g_counter[16];

void aes128_ctr_init(uint8_t *key, uint8_t *counter) {
  memcpy(g_counter, counter, 16);
  /* Decrementing the counter so that initial value will match without
     special logic during encryption */
  block_decr(g_counter);
  aes128_init(key);
}

uint8_t *aes128_ctr_evolve_counter(void) {
  block_incr(g_counter);
  return g_counter;
}

void aes128_ctr(uint8_t *dest, uint8_t *msg, uint32_t msg_len) {
  uint32_t num_blocks = msg_len / 16 + (msg_len % 16 ? 1 : 0);
  for (uint32_t i = 0; i < num_blocks; i++) {
    uint8_t keystream[16];
    aes128_ecb(keystream, aes128_ctr_evolve_counter());
    block_xor(dest+(i*16), keystream, msg+(i*16));
  }
}
