/*
 * Copyright (c) - All Rights Reserved.
 * 
 * See the LICENCE file for more information.
 */

#pragma once
#include "pic.h"

const struct PicDriver* apic_GetDriver();
bool apic_Probe();
void apic_Initialize(uint8_t offsetPic1, uint8_t offsetPic2, bool autoEoi);
void apic_Disable();
void apic_SendEndOfInterrupt(int irq);
void apic_Mask(int irq);
void apic_Unmask(int irq);