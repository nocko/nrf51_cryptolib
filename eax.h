#pragma once

void aes128_eax(uint8_t *, uint8_t *, uint32_t, uint8_t *, uint32_t,
                uint_fast8_t);
void aes128_eax_cache_ad(uint8_t *dest, uint8_t *header, uint32_t header_len,
			 uint8_t *msg, uint32_t msg_len, uint_fast8_t tag_len);
void aes128_eax_init(const uint8_t *, uint8_t *);
void eax_dump_state(void);
