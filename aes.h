#pragma once

#include "block.h"

#include <stdint.h>

typedef struct {
  uint8_t key[16];
  uint8_t in[16];
  uint8_t out[16];
} ecbdata_t;


void aes128_init(uint8_t *);
block_t aes128_ecb(block_t const * const);
