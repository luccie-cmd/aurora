/*
 * Copyright (c) - All Rights Reserved.
 * 
 * See the LICENCE file for more information.
 */

#pragma once
#include "pic.h"

const struct PicDriver* i8259_GetDriver();
bool i8259_Probe();
void i8259_Initialize(uint8_t offsetPic1, uint8_t offsetPic2, bool autoEoi);
void i8259_Disable();
void i8259_SendEndOfInterrupt(int irq);
void i8259_Mask(int irq);
void i8259_Unmask(int irq);