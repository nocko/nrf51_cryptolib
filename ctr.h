#pragma once

#include <stdint.h>

void aes128_ctr(uint8_t *, uint8_t *, uint32_t);
void aes128_ctr_init(uint8_t *, uint8_t *);
