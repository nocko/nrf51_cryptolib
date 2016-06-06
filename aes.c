#include "aes.h"
#include "block.h"

#ifndef HOST_BUILD
#include "nrf.h"
#else
#include <openssl/evp.h>
#endif /* HOST_BUILD */

#include <string.h>

static ecbdata_t g_ecbdata;

void aes128_init(uint8_t *key) {
  memmove(g_ecbdata.key, key, 16);
#ifndef HOST_BUILD
  NRF_ECB->ECBDATAPTR = (uint32_t)&g_ecbdata;
#endif /* HOST_BUILD */
  return;
}

void aes128_ecb(uint8_t *dest, uint8_t const * const in) {
#ifdef HOST_BUILD
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, g_ecbdata.key, NULL);

  int outlen;//tmplen;
  EVP_EncryptUpdate(ctx, g_ecbdata.out, &outlen, in, 16);
  //EVP_EncryptFinal_ex(ctx, g_ecbdata.out + outlen, &tmplen);

  /* Why the fuck does this segfault... I don't care. It's for testing
     on the host only. Let it leak */
  //EVP_CIPHER_CTX_free(ctx);
#else
  memmove(g_ecbdata.in, in, 16);
  NRF_ECB->TASKS_STARTECB = 1;
  while (!NRF_ECB->EVENTS_ENDECB);
  NRF_ECB->EVENTS_ENDECB = 0;
#endif /* HOST_BUILD */
  memmove(dest, g_ecbdata.out, 16);
  return;
}
