#pragma once

#include <stdbool.h>
#include <stdint.h>

bool block_eq(uint8_t const * const, uint8_t const * const);
void block_print(char const * const, uint8_t const * const);
void block_shiftl(uint8_t *, uint8_t * const, uint_fast8_t);
void block_shiftr(uint8_t *, uint8_t * const, uint_fast8_t);
void block_xor(uint8_t *, uint8_t * const, uint8_t * const);

