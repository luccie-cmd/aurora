/*
 * Copyright (c) - All Rights Reserved.
 * 
 * See the LICENCE file for more information.
 */

#pragma once
#include <stdint.h>
// LCG parameters
#define RAND_MAX (pow(2, 32))
#define LCG_A 1664525
#define LCG_C 1013904223

uint64_t pow(uint64_t base, uint64_t exponent);
uint32_t rand();
uint32_t randRange(uint32_t max);
void seed(uint32_t seed);
