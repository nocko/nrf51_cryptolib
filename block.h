#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef union {
  uint32_t ui32[4];
  uint8_t ui8[16];
} block_t;

bool    block_cmp(block_t const * const, block_t const * const);
void    block_print(char const * const, block_t const * const);
block_t block_shiftl(block_t const * const, uint_fast8_t);
block_t block_shiftr(block_t const * const, uint_fast8_t);
block_t block_xor(block_t const * const, block_t const * const);

