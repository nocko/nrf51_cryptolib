#pragma once

#include <stdbool.h>
#include <strings.h>

#include "block.h"

block_t cmac_aes128(uint8_t *, size_t);
void    cmac_aes128_expand_key(block_t *, block_t *);
void    cmac_aes128_init(block_t *);
void    cmac_truncate_tag(uint8_t *, block_t *, uint_fast8_t);
