/*
 * Copyright (c) - All Rights Reserved.
 * 
 * See the LICENCE file for more information.
 */

#include "math.h"
#include <errno.h>
#include <stdio.h>
#include <arch/io/io.h>

static uint32_t next_rand = 1;

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
    next_rand = (LCG_A * next_rand + LCG_C) % RAND_MAX;
    return next_rand;
}

uint32_t randRange(uint32_t max){
    uint32_t value = rand() % (max + 1);
    return rand() % (max + 1);
}

void seed(uint32_t seed){
    next_rand = seed;
}