/*
 * Copyright (c) - All Rights Reserved.
 * 
 * See the LICENCE file for more information.
 */

#pragma once
#include <stddef.h>

enum ReclaimType{
    ACPI_RECLAIM,
    BOOTLOADER_RECLAIM,
};

void InitMMU();
void CheckMemorySize();
void PrintMemoryInfo();
void NotifyReclaimed(enum ReclaimType type, size_t length);
void SetBitMap();
void GetTotalMMSize();