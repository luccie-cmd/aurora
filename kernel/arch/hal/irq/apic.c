/*
 * Copyright (c) - All Rights Reserved.
 * 
 * See the LICENCE file for more information.
 */

#include "apic.h"
#include <arch/io/io.h>
#include <debug.h>

#define APIC_BASE_MSR          0x1B
#define APIC_ENABLE            0x800
#define APIC_REG_EOI           0xB0
#define APIC_REG_SIV           0xF0
#define APIC_REG_TPR           0x80
#define APIC_REG_LVT_TIMER     0x320
#define APIC_REG_LVT_LINT0     0x350
#define APIC_REG_LVT_LINT1     0x360
#define APIC_REG_LVT_ERROR     0x370
#define APIC_REG_LVT_CMCI      0x2F0
#define APIC_REG_ICR_LOW       0x300
#define APIC_REG_ICR_HIGH      0x310
#define APIC_REG_IRR           0x200
#define APIC_REG_ISR           0x100
#define APIC_REG_TMR           0x180

static void write_apic(uint32_t reg, uint32_t value) {
    volatile uint32_t* apic = (uint32_t*)(read_msr(APIC_BASE_MSR) & 0xFFFFF000);
    apic[reg >> 2] = value;
}

// Function to read from an APIC register
static uint32_t read_apic(uint32_t reg) {
    volatile uint32_t* apic = (uint32_t*)(read_msr(APIC_BASE_MSR) & 0xFFFFF000);
    return apic[reg >> 2];
}

bool apic_Probe(){
    uint32_t eax, ebx, ecx, edx;
    cpuid(1, &eax, &ebx, &ecx, &edx);
    return edx & CPUID_FEAT_EDX_APIC;
}
void apic_Initialize(uint8_t offsetPic1, uint8_t offsetPic2, bool autoEoi){
    write_msr(APIC_BASE_MSR, read_msr(APIC_BASE_MSR) | APIC_ENABLE);
    write_apic(APIC_REG_SIV, 0x1FF);
    if (autoEoi) {
        DebugLog("Fuck off with your autoEoi you stupid dev\n");
    }
}
void apic_Disable(){
    write_apic(APIC_REG_SIV, read_apic(APIC_REG_SIV) & ~0x100);
}
void apic_SendEndOfInterrupt(int irq){
    write_apic(APIC_REG_EOI, 0);
}
void apic_Mask(int irq){
    uint32_t reg;
    switch (irq) {
        case 0:
            reg = APIC_REG_LVT_TIMER;
            break;
        case 1:
            reg = APIC_REG_LVT_LINT0;
            break;
        case 2:
            reg = APIC_REG_LVT_LINT1;
            break;
        case 3:
            reg = APIC_REG_LVT_ERROR;
            break;
        default:
            return; // Unsupported IRQ
    }
    write_apic(reg, read_apic(reg) | 0x10000);
}
void apic_Unmask(int irq){
    uint32_t reg;
    switch (irq) {
        case 0:
            reg = APIC_REG_LVT_TIMER;
            break;
        case 1:
            reg = APIC_REG_LVT_LINT0;
            break;
        case 2:
            reg = APIC_REG_LVT_LINT1;
            break;
        case 3:
            reg = APIC_REG_LVT_ERROR;
            break;
        default:
            return; // Unsupported IRQ
    }
    write_apic(reg, read_apic(reg) & ~0x10000); // Clear the mask bit
}

static struct PicDriver driver = {
    .Name = "APIC",
    .Probe = apic_Probe,
    .Initialize = apic_Initialize,
    .Disable = apic_Disable,
    .SendEndOfInterrupt = apic_SendEndOfInterrupt,
    .Mask = apic_Mask,
    .Unmask = apic_Unmask,
};

const struct PicDriver* apic_GetDriver(){
    return &driver;
}