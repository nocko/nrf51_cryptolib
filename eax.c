#include <stdio.h>
#include <string.h>

#include "block.h"
#include "cmac.h"
#include "ctr.h"

static uint8_t g_nonce[16] = {0};

void aes128_eax_init(uint8_t *key, uint8_t *nonce) {
  cmac_aes128_init(key);
  aes128_ctr_init(key, nonce);
  memcpy(g_nonce, nonce, 16);
  return;
}

static void eax_omac(uint8_t *dest, uint8_t *msg, uint32_t msg_len, uint32_t t) {
  uint8_t buf[msg_len+16];
  memset(buf, 0, msg_len+16);
  buf[15] = (uint8_t)t;
  buf[14] = (uint8_t)t >> 8;
  buf[13] = (uint8_t)t >> 16;
  buf[12] = (uint8_t)t >> 24;
  memcpy(buf+16, msg, msg_len);
  //printf("\nOMAC: ");
  /* for (int i = 0; i < msg_len+16; i++) { */
  /*   if (!(i % 4)) { */
  /*     printf(" "); */
  /*   } */
  /*   printf("%.2x", buf[i]); */
  /* } */
  /* printf("\n"); */
  cmac_aes128(dest, buf, msg_len+16, 16);
  return;
}

void aes128_eax(uint8_t *dest, uint8_t *header, uint32_t header_len,
		uint8_t *msg, uint32_t msg_len, uint_fast8_t tag_len) {
  uint8_t nonce_cmac[16],
    header_cmac[16],
    ciphertext_cmac[16],
    ciphertext[msg_len+16];
  
  block_print("Nonce", g_nonce);
  eax_omac(nonce_cmac, g_nonce, 16, 0);
  block_print("CMAC nonce", nonce_cmac);
  eax_omac(header_cmac, header, header_len, 1);
  block_print("CMAC header", header_cmac);
  aes128_ctr(ciphertext, msg, msg_len);
  printf("Ciphertext: ");
  for (int i = 0; i < msg_len; i++) {
    if (!(i % 4)) {
      printf(" ");
    }
    printf("%.2x", ciphertext[i]);
  }
  printf("\n");
  eax_omac(ciphertext_cmac, ciphertext, msg_len, 2);
  block_print("CMAC ciphertext", ciphertext_cmac);
  block_xor(nonce_cmac, nonce_cmac, header_cmac);
  block_xor(nonce_cmac, nonce_cmac, ciphertext_cmac);
  block_print("Tag", nonce_cmac);
  memcpy(dest, ciphertext, msg_len);
  memcpy(dest+msg_len, nonce_cmac, tag_len);
  return;
}
