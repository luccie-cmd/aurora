/*
 * Copyright (c) - All Rights Reserved.
 * 
 * See the LICENCE file for more information.
 */

#pragma once
#include <stdint.h>
#include <arch/io/io.h>

typedef void (*IRQHandler)(Registers* regs);

void InitIRQ();