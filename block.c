/* Utility functions for manipulating block_t structures; 128bit
   blocks of data for AES & CMAC */

#include "block.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

bool block_cmp(block_t const * const a, block_t const * const b) {
  /* Compares two blocks, return true if they are identical, else
     false */
  for (uint_fast8_t i = 0; i < 4; i++) {
    if (a->ui32[i] != b->ui32[i]) {
      return false;
    }
  }
  return true;
}

#ifdef HOST_BUILD
void block_print(char const * const label,
		 block_t const * const b) {
  if (label != NULL) {
  printf("\n%s: ", label);
  } else {
    printf("\n");
  }
  for(int i = 0; i < 16; i++) {
    printf("%.2x", b->ui8[i]);
    if (!((i+1) % 4)) {
      printf(" ");
    }
  }
  printf("\n");
  return;
}
#endif /* HOST_BUILD */

block_t block_xor(block_t const * const a, block_t const * const b) {
  /* Returns XOR of two block_t */
  block_t c;
  for (uint_fast8_t i = 0; i < 4; i++) {
    c.ui32[i] = a->ui32[i] ^ b->ui32[i];
  }
  return c;
}

block_t block_shiftr(block_t const * const a, uint_fast8_t const num) {
  /* Implements '>>' for block_t */
  block_t c;
  uint_fast8_t n = (num <= 8 ? num : 8);
  for (int_fast8_t i = 15; i >= 0; i--) {
    /* Shift right from LSB to MSB */
    c.ui8[i] = a->ui8[i] >> n;
    if (i != 0) {
      /* If theres a more significant uint32, we need to pull in its lsb */
      c.ui8[i] |= (a->ui8[i-1] << (8 - n));
    }
  }
  return (num - n == 0 ? c : block_shiftr(&c, num - n));
}

block_t block_shiftl(block_t const * const a, uint_fast8_t const num) {
  /* Implements '<<' for block_t */
  block_t c;
  uint_fast8_t n = (num <= 8 ? num : 8);
  for (int_fast8_t i = 0; i < 16; i++) {
    /* Shift right from LSB to MSB */
    c.ui8[i] = a->ui8[i] << n;
    if (i != 15) {
      /* If theres a less significant uint32, we need to pull in its msb */
      c.ui8[i] |= (a->ui8[i+1] >> (8 - n));
    }
  }
  return (num - n == 0 ? c : block_shiftl(&c, num - n));
 }

