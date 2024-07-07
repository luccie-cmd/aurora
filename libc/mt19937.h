/*
 * Copyright (c) - All Rights Reserved.
 */

#pragma once
#include <stdint.h>

#define MT19937_N 624
#define MT19937_M 397
#define MT19937_MATRIX_A 0x9908b0dfUL
#define MT19937_UPPER_MASK 0x80000000UL
#define MT19937_LOWER_MASK 0x7fffffffUL

void MT19937_twist();
uint32_t MT19937_extract_number();
void MT19937_init_genrand(uint32_t seed);