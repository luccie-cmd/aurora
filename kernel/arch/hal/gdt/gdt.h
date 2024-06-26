#pragma once
#include <stdint.h>
#include "tss.h"

#define NULL_ENTRY  0
#define KERNEL_CODE 1
#define KERNEL_DATA 2
#define USER_CODE   3
#define USER_DATA   4
#define TSS         5

typedef struct {
    uint16_t LimitLow;
    uint16_t BaseLow;
    uint8_t BaseMiddle;
    uint8_t Access;
    uint8_t FlagsLimitHi;
    uint8_t BaseHigh;
} __attribute__((packed)) GdtEntry;


typedef struct GdtDescriptor{
    uint16_t Limit;                     // sizeof(Gdt) - 1
    GdtEntry* Ptr;
} __attribute__((packed)) GdtDescriptor;

typedef enum
{
    GDT_ACCESS_CODE_READABLE                = 0x02,
    GDT_ACCESS_DATA_WRITEABLE               = 0x02,

    GDT_ACCESS_CODE_CONFORMING              = 0x04,
    GDT_ACCESS_DATA_DIRECTION_NORMAL        = 0x00,
    GDT_ACCESS_DATA_DIRECTION_DOWN          = 0x04,

    GDT_ACCESS_DATA_SEGMENT                 = 0x10,
    GDT_ACCESS_CODE_SEGMENT                 = 0x18,
    GDT_ACCESS_DESCRIPTOR_TSS               = 0x00,

    GDT_ACCESS_RING0                        = 0x00,
    GDT_ACCESS_RING1                        = 0x20,
    GDT_ACCESS_RING2                        = 0x40,
    GDT_ACCESS_RING3                        = 0x60,

    GDT_ACCESS_PRESENT                      = 0x80,

} GDT_ACCESS;

typedef enum 
{
    GDT_FLAG_64BIT                          = 0x20,
    GDT_FLAG_32BIT                          = 0x40,
    GDT_FLAG_16BIT                          = 0x00,

    GDT_FLAG_GRANULARITY_1B                 = 0x00,
    GDT_FLAG_GRANULARITY_4K                 = 0x80,
} GDT_FLAGS;

#define GDT_ENTRY(base, limit, access, flags) \
    {                              \
        (limit) & 0xFFFF,         \
        (base) & 0xFFFF,           \
        ((base) >> 16) & 0xFF,  \
        (access),                   \
        (((limit) >> 16) & 0x0F) | ((flags) << 4), \
        ((base) >> 24) & 0xFF,    \
    }

void InitGDT();