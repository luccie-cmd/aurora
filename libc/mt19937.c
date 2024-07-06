/*
 * Copyright (c) - All Rights Reserved.
 */

#include "mt19937.h"
static uint32_t mt[MT19937_N];
int mti = MT19937_N + 1;

void MT19937_twist(){
    for (int i = 0; i < MT19937_N; ++i) {
        uint32_t upper_bit = (mt[i] & MT19937_UPPER_MASK);
        uint32_t lower_bits = (mt[(i + 1) % MT19937_N] & MT19937_LOWER_MASK);
        mt[i] = mt[(i + MT19937_M) % MT19937_N] ^ (upper_bit | lower_bits >> 1);
        if (mt[i] & 1) {
            mt[i] ^= MT19937_MATRIX_A;
        }
    }
    mti = 0;
}
uint32_t MT19937_extract_number(){
    if (mti >= MT19937_N) {
        MT19937_twist();
    }

    uint32_t y = mt[mti++];
    y ^= (y >> 11);
    y ^= ((y << 7) & 0x9d2c5680UL);
    y ^= ((y << 15) & 0xefc60000UL);
    y ^= (y >> 18);

    return y;
}
void MT19937_init_genrand(uint32_t seed){
    mt[0] = seed;
    for (mti = 1; mti < MT19937_N; ++mti) {
        mt[mti] = (1812433253UL * (mt[mti - 1] ^ (mt[mti - 1] >> 30)) + mti);
    }
}