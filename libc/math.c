/*
 * Copyright (c) - All Rights Reserved.
 * 
 * See the LICENCE file for more information.
 */

#include <math.h>
#include <errno.h>
#include <stdio.h>
#include <mt19937.h>

static uint32_t next_rand = 1;
extern void Panic();

uint64_t pow(uint64_t base, uint64_t exponent){
    if (exponent == 0) {
        errno = EINVAL;
        Panic();
    }

    uint64_t result = 1;
    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result *= base;
        }
        base *= base;
        exponent /= 2;
    }

    return result;
}

uint32_t rand(){
#ifdef MT19937
    return MT19937_extract_number();
#else
    return (LCG_A * next_rand + LCG_C) % RAND_MAX;
#endif
}

uint32_t randRange(uint32_t max){
    return rand() % (max + 1);
}

void seed(uint32_t seed){
#ifdef MT19937
    MT19937_init_genrand(seed);
#else
    next_rand = seed;
#endif
}