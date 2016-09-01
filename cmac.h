#pragma once

#include <stdbool.h>
#include <strings.h>

#include "block.h"

void cmac_aes128(uint8_t *, uint8_t *, size_t, uint_fast8_t);
void cmac_aes128_expand_key(uint8_t const *const, uint8_t *, uint8_t *);
void cmac_aes128_init(const uint8_t *);
void cmac_get_subkeys(uint8_t *);
