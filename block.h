#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define block_eq(x, y) (!memcmp(x, y, 16))

void block_decr(uint8_t *);
void block_incr(uint8_t *);
void block_print(char const *const, uint8_t const *const);
void block_print_bytes(char const *const, uint8_t const *const, uint32_t);
void block_shiftl(uint8_t *, uint8_t *const, uint_fast8_t);
void block_shiftr(uint8_t *, uint8_t *const, uint_fast8_t);
void block_xor(uint8_t *, uint8_t *const, uint8_t *const);
