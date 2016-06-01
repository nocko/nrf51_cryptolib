#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "aes.h"
#include "block.h"
#include "cmac.h"

static const block_t zeros = {.ui32={0,0,0,0}};

static block_t g_k[2];

void cmac_aes128_init(block_t *key) {
  /* Initialize AES engine and cache subkeys */
  aes128_init(key->ui8);
  cmac_aes128_expand_key(key, g_k);
}   

void cmac_aes128_expand_key(block_t *key, block_t *out) {
  /* Generate two required subkeys according to NIST 800-38B */
  block_t *k1 = out,
    *k2 = (out+1);
  
  block_t l = {0},
    Rb = {.ui8={0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0x87}};
  
  l = aes128_ecb(&zeros);
  if ((l.ui8[0] >> 7) == 0) {
    *k1 = block_shiftl(&l, 1);
  } else {
    block_t tmp = block_shiftl(&l, 1);
    *k1 = block_xor(&tmp, &Rb);
  }
  if (!(k1->ui8[0] >> 7)) {
    *k2 = block_shiftl(k1, 1);
  } else {
    block_t tmp = block_shiftl(k1, 1);
    *k2 = block_xor(&tmp, &Rb);
  }
}

void cmac_truncate_tag(uint8_t *dest, block_t *tag, uint_fast8_t tag_len_bits) {
  /* Copy `tag_len_bits` of the tag's most significant bits into to
     dest buffer. This is the truncation method defined in NIST
     800-38B */
  uint_fast8_t num_bytes = tag_len_bits / 8,
    last_byte_mask = 0xff << (8 - tag_len_bits % 8);
  memcpy(dest, tag->ui8, num_bytes);
  dest[num_bytes] = tag->ui8[num_bytes] & last_byte_mask;
} 

/* Address of the first uint8_t in a particular message block */
#define BLOCK(x) (&alt_msg[x-1])

block_t cmac_aes128(uint8_t *msg, size_t msg_len) {
  /* Returns a block_t containing the entire CMAC-AES128 tag */
  block_t *k1 = &g_k[0],
    *k2 = &g_k[1];

  /* Simulate ceiling integer division by adding a block if remainder */
  uint_fast16_t num_blocks = msg_len / 16 + (msg_len % 16 ? 1 : 0);
  bool last_block_complete = !(msg_len % 16 ? 1 : 0);
  if (msg_len == 0) {
    num_blocks = 1;
    last_block_complete = false;
  }
  
  block_t alt_msg[num_blocks];
  memset(&alt_msg, 0, num_blocks*16);
  memcpy(&alt_msg, msg, msg_len);

  if (!last_block_complete) {
    /* Padding is single 1 bit, run out on 0s.. find the next byte,
       set it to 0b1000000 */
    alt_msg[num_blocks-1].ui8[msg_len % 16] = 0x80;
    alt_msg[num_blocks-1] = block_xor(BLOCK(num_blocks), k2);
  } else {
    alt_msg[num_blocks-1] = block_xor(BLOCK(num_blocks), k1);
  }
  
  block_t x = { .ui32={0, 0, 0, 0}},
    y = { .ui32={0, 0, 0, 0}};

  for (uint32_t i = 1; i <= num_blocks - 1; i++) {
    y = block_xor(&x, BLOCK(i));
    x = aes128_ecb(&y);
  }
  y = block_xor(&x, BLOCK(num_blocks));
  block_t tag = aes128_ecb(&y);
  return tag;
}

