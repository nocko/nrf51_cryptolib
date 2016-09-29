/* CMAC Key Derivation Function

   HKDF, but with CMAC

   Based on ietf draft-agl-ckdf-00

   Shawn Nock - <nock@nocko.se>
*/

void ckdf_extract(uint8_t *, const uint8_t *, const uint8_t *, uint32_t);
void ckdf_expand(uint8_t *, uint8_t *, uint8_t *, uint32_t, uint32_t);
