#pragma once

void aes128_eax(uint8_t *, uint8_t *, uint32_t, uint8_t *, uint32_t, uint_fast8_t);
void aes128_eax_init(uint8_t *, uint8_t *);
void eax_dump_state(void);
