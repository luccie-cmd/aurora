/*
 * Copyright (c) - All Rights Reserved.
 * 
 * See the LICENCE file for more information.
 */

#pragma once
#include <stdint.h>
#include <stdbool.h>
// Used for all programmable interrupt controllers (apic, i8259)
struct PicDriver{
    const char* Name;
    bool (*Probe)();
    void (*Initialize)(uint8_t offsetPic1, uint8_t offsetPic2, bool autoEoi);
    void (*Disable)();
    void (*SendEndOfInterrupt)(int irq);
    void (*Mask)(int irq);
    void (*Unmask)(int irq);
};