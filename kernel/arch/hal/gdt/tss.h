#pragma once
#include <stdint.h>

typedef struct {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_mid;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_hi;
    uint32_t base_upper;
    uint32_t reserved;
} __attribute__((packed)) TssEntry;

extern void loadTSS(int entryN);