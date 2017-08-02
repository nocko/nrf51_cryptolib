#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "aes.h"
#include "block.h"
#include "cmac.h"

static const uint8_t zeros[16] = {0};

static uint8_t g_k1[16], g_k2[16];

#ifdef HOST_BUILD
void cmac_get_subkeys(uint8_t *dest) {
    /* Testing stub to get subkeys for algo check */
    memcpy(dest, g_k1, 16);
    memcpy(dest + 16, g_k2, 16);
    return;
}
#endif /* HOST_BUILD */

void cmac_aes128_init(const uint8_t *key) {
    /* Initialize AES engine and cache subkeys */
    if (key != NULL) {
        aes128_init(key);
        cmac_aes128_expand_key(key, g_k1, g_k2);
    }
}

void cmac_aes128_expand_key(uint8_t const *const key, uint8_t *k1,
                            uint8_t *k2) {
    /* Generate two required subkeys according to NIST 800-38B */
    uint8_t Rb[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x87};

    uint8_t *l = aes128_ecb(zeros);
    if ((l[0] >> 7) == 0) {
        block_shiftl(k1, l, 1);
    } else {
        uint8_t tmp[16];
        block_shiftl(tmp, l, 1);
        block_xor(k1, tmp, Rb);
    }
    if (!(k1[0] >> 7)) {
        block_shiftl(k2, k1, 1);
    } else {
        uint8_t tmp[16];
        block_shiftl(tmp, k1, 1);
        block_xor(k2, tmp, Rb);
    }
}

static void cmac_truncate(uint8_t *dest, uint8_t *tag, uint_fast8_t tag_len) {
    memmove(dest, tag, tag_len);
}

void cmac_aes128(uint8_t *dest, uint8_t *msg, size_t msg_len,
                 uint_fast8_t tag_len) {
    /* Simulate ceiling integer division by adding a block if remainder */
    size_t num_blocks = (msg_len >> 4) + (msg_len & 15 ? 1 : 0);
    bool last_block_complete = !(msg_len & 15 ? 1 : 0);
    if (msg_len == 0) {
        num_blocks = 1;
        last_block_complete = false;
    }

    size_t padded_len = num_blocks << 4;
    uint8_t alt_msg[padded_len],
        *last_block = &alt_msg[padded_len - 16];
    memset(alt_msg+msg_len, 0, padded_len - msg_len);
    memcpy(alt_msg, msg, msg_len);

    if (!last_block_complete) {
        /* Padding is single 1 bit, run out on 0s.. find the next byte,
           set it to 0b1000000 */
        alt_msg[msg_len] = 0x80;
        block_xor(last_block, last_block, g_k2);
    } else {
        block_xor(last_block, last_block, g_k1);
    }

    uint8_t *x = (uint8_t *)zeros, y[16] = {0};

    for (uint32_t i = 0; i < num_blocks; i++) {
        uint8_t *block = &alt_msg[i << 4];
        block_xor(y, x, block);
        x = aes128_ecb(y);
    }
    cmac_truncate(dest, x, tag_len);
    return;
}
