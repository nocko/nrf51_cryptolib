#include "aes.h"
#include "block.h"

#ifndef HOST_BUILD
#include "nrf.h"
#else
#include <openssl/aes.h>
#endif /* HOST_BUILD */

#include <string.h>

static ecbdata_t g_ecbdata;

void aes128_init(uint8_t *key) {
    if (key != NULL) {
        memmove(g_ecbdata.key, key, 16);
#ifndef HOST_BUILD
        NRF_ECB->ECBDATAPTR = (uint32_t)&g_ecbdata;
#endif /* HOST_BUILD */
    }
    return;
}

void aes128_ecb(uint8_t *dest, uint8_t const *const in) {
#ifdef HOST_BUILD
    AES_KEY key;
    AES_set_encrypt_key(g_ecbdata.key, 128, &key);
    AES_encrypt(in, g_ecbdata.out, &key);
#else
    memmove(g_ecbdata.in, in, 16);
    NRF_ECB->TASKS_STARTECB = 1;
    while (!NRF_ECB->EVENTS_ENDECB)
        ;
    NRF_ECB->EVENTS_ENDECB = 0;
#endif /* HOST_BUILD */
    memmove(dest, g_ecbdata.out, 16);
    return;
}

#ifdef HOST_BUILD
void aes_dump_state(void) {
    block_print("Key", g_ecbdata.key);
}
#endif /* HOST_BUILD */
