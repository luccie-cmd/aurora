#pragma once
#include <stdint.h>

#define IDT_ENTRIES 256

typedef struct {
    uint16_t offset_low;    // offset bits 0-15
    uint16_t selector;      // a code segment selector in GDT or LDT
    uint8_t  ist;           // bits 0-2 hold Interrupt Stack Table offset, rest of bits zero.
    uint8_t  type_attr;     // type and attributes
    uint16_t offset_mid;    // offset bits 16-31
    uint32_t offset_high;   // offset bits 32-63
    uint32_t zero;          // reserved
} __attribute__((packed)) IdtEntry;

typedef struct {
    uint16_t limit;
    IdtEntry* base;
} __attribute__((packed)) IdtDescriptor;

void InitIDT();
void DisableGateIdt(int interrupt);
void EnableGateIdt(int interrupt);
void SetGateIdt(int interrupt, void* base, uint16_t segmentDescriptor, uint8_t flags);