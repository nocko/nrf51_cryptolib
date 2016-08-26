/* CMAC Key Derivation Function

   HKDF, but with CMAC

   Based on ietf draft-agl-ckdf-00

   Shawn Nock - <nock@nocko.se>
*/

#include <stdint.h>
#include <stdio.h>

#include "cmac.h"

void ckdf_extract(uint8_t *prk, uint8_t *salt, uint8_t *ikm,
		  uint32_t ikm_len) {
  uint8_t zeros[16] = {0};
  if (salt == NULL) {
    cmac_aes128_init(zeros);
  } else {
    cmac_aes128_init(salt);
  }
  cmac_aes128(prk, ikm, ikm_len, 16);
}

void ckdf_expand(uint8_t *okm, uint8_t *prk, uint8_t *info,
		 uint32_t info_len,  uint32_t okm_len) {
  uint32_t num_blocks = okm_len / 16;
  /* Simulate ceiling division instead of C std floor */
  uint8_t remainder = okm_len % 16;
  if (remainder) {
    num_blocks++;
  }
  cmac_aes128_init(prk);
  uint8_t t[16+info_len+1];
  uint8_t *initial = &t[16];
  memset(t, 0, 16);
  for (uint32_t i = 0; i < num_blocks; i++) {
    *(initial+info_len) = i+1;
    if (i == 0) {
      memcpy(initial, info, info_len);
      cmac_aes128(t, initial, info_len+1, 16);
    } else {
      block_print_bytes("IN", t, sizeof(t));
      cmac_aes128(t, t, sizeof(t), 16);
      block_print_bytes("OUT", t, sizeof(t));
    }
    printf("T%d: ", i+1);
    block_print(NULL, t);
    if (i == num_blocks - 1 && remainder) {
      memcpy(okm+16*i, t, remainder);
    } else {
      memcpy(okm+16*i, t, 16);
    }
  }
  return;
}
