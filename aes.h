#pragma once

#include "block.h"

#include <stdint.h>

typedef struct {
  uint8_t key[16];
  uint8_t in[16];
  uint8_t out[16];
} ecbdata_t;


void aes128_init(uint8_t *);
void aes128_ecb(uint8_t *, uint8_t const * const);
void aes_dump_state(void);
