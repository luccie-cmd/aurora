#include "idt.h"

extern void loadIDT(IdtDescriptor* descriptor);

IdtEntry idt[IDT_ENTRIES];
IdtDescriptor idtDescriptor;

void DisableGateIdt(int interrupt){
    idt[interrupt].type_attr &= 0x80;
}
void EnableGateIdt(int interrupt){
    idt[interrupt].type_attr |= 0x80;
}
void SetGateIdt(int interrupt, void* base, uint16_t segmentDescriptor, uint8_t flags){
    idt[interrupt].offset_low = (uint64_t)base & 0xFFFF;
    idt[interrupt].selector = segmentDescriptor;
    idt[interrupt].ist = 0;
    idt[interrupt].type_attr = flags;
    idt[interrupt].offset_mid = ((uint64_t)base >> 16) & 0xFFFF;
    idt[interrupt].offset_high = ((uint64_t)base >> 32) & 0xFFFFFFFF;
    idt[interrupt].zero = 0;
}

void InitIDT() {
    idtDescriptor.limit = sizeof(idt) - 1;
    idtDescriptor.base = idt;
    // Load the IDT
    loadIDT(&idtDescriptor);
}