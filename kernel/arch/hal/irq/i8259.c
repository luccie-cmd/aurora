/*
 * Copyright (c) - All Rights Reserved.
 * 
 * See the LICENCE file for more information.
 */
#include "i8259.h"
#include <arch/io/io.h>

#define PIC1_COMMAND_PORT           0x20
#define PIC1_DATA_PORT              0x21
#define PIC2_COMMAND_PORT           0xA0
#define PIC2_DATA_PORT              0xA1

enum {
    PIC_ICW1_ICW4           = 0x01,
    PIC_ICW1_SINGLE         = 0x02,
    PIC_ICW1_INTERVAL4      = 0x04,
    PIC_ICW1_LEVEL          = 0x08,
    PIC_ICW1_INITIALIZE     = 0x10
} PIC_ICW1;

enum {
    PIC_ICW4_8086           = 0x1,
    PIC_ICW4_AUTO_EOI       = 0x2,
    PIC_ICW4_BUFFER_MASTER  = 0x4,
    PIC_ICW4_BUFFER_SLAVE   = 0x0,
    PIC_ICW4_BUFFERRED      = 0x8,
    PIC_ICW4_SFNM           = 0x10,
} PIC_ICW4;


enum {
    PIC_CMD_END_OF_INTERRUPT    = 0x20,
    PIC_CMD_READ_IRR            = 0x0A,
    PIC_CMD_READ_ISR            = 0x0B,
} PIC_CMD;


static uint16_t g_PicMask = 0xffff;
static bool g_AutoEoi = false;

void i8259_SetMask(uint16_t newMask) 
{
    g_PicMask = newMask;
    outb(PIC1_DATA_PORT, g_PicMask & 0xFF);
    iowait();
    outb(PIC2_DATA_PORT, g_PicMask >> 8);
    iowait();
}

uint16_t i8259_GetMask() 
{
    return inb(PIC1_DATA_PORT) | (inb(PIC2_DATA_PORT) << 8);
}

void i8259_Configure(uint8_t offsetPic1, uint8_t offsetPic2, bool autoEoi)
{
    // Mask everything
    i8259_SetMask(0xFFFF);

    // initialization control word 1
    outb(PIC1_COMMAND_PORT, PIC_ICW1_ICW4 | PIC_ICW1_INITIALIZE);
    iowait();
    outb(PIC2_COMMAND_PORT, PIC_ICW1_ICW4 | PIC_ICW1_INITIALIZE);
    iowait();

    // initialization control word 2 - the offsets
    outb(PIC1_DATA_PORT, offsetPic1);
    iowait();
    outb(PIC2_DATA_PORT, offsetPic2);
    iowait();

    // initialization control word 3
    outb(PIC1_DATA_PORT, 0x4);             // tell PIC1 that it has a slave at IRQ2 (0000 0100)
    iowait();
    outb(PIC2_DATA_PORT, 0x2);             // tell PIC2 its cascade identity (0000 0010)
    iowait();

    // initialization control word 4
    uint8_t icw4 = PIC_ICW4_8086;
    if (autoEoi) {
        icw4 |= PIC_ICW4_AUTO_EOI;
    }

    outb(PIC1_DATA_PORT, icw4);
    iowait();
    outb(PIC2_DATA_PORT, icw4);
    iowait();

    // mask all interrupts until they are enabled by the device driver
    i8259_SetMask(0xFFFF);
}

void i8259_SendEndOfInterrupt(int irq)
{
    if (irq >= 8)
        outb(PIC2_COMMAND_PORT, PIC_CMD_END_OF_INTERRUPT);
    outb(PIC1_COMMAND_PORT, PIC_CMD_END_OF_INTERRUPT);
}

void i8259_Disable()
{
    i8259_SetMask(0xFFFF);
}

void i8259_Mask(int irq)
{
    i8259_SetMask(g_PicMask | (1 << irq));
}

void i8259_Unmask(int irq)
{
    i8259_SetMask(g_PicMask & ~(1 << irq));
}

uint16_t i8259_ReadIrqRequestRegister()
{
    outb(PIC1_COMMAND_PORT, PIC_CMD_READ_IRR);
    outb(PIC2_COMMAND_PORT, PIC_CMD_READ_IRR);
    return ((uint16_t)inb(PIC2_COMMAND_PORT)) | (((uint16_t)inb(PIC2_COMMAND_PORT)) << 8);
}

uint16_t i8259_ReadInServiceRegister()
{
    outb(PIC1_COMMAND_PORT, PIC_CMD_READ_ISR);
    outb(PIC2_COMMAND_PORT, PIC_CMD_READ_ISR);
    return ((uint16_t)inb(PIC2_COMMAND_PORT)) | (((uint16_t)inb(PIC2_COMMAND_PORT)) << 8);
}

bool i8259_Probe()
{
    i8259_Disable();
    i8259_SetMask(0x1337);
    return i8259_GetMask() == 0x1337;
}

static const struct PicDriver driver = {
    .Name = "I8259 PIC",
    .Probe = &i8259_Probe,
    .Initialize = &i8259_Configure,
    .Disable = &i8259_Disable,
    .SendEndOfInterrupt = &i8259_SendEndOfInterrupt,
    .Mask = &i8259_Mask,
    .Unmask = &i8259_Unmask,
};

const struct PicDriver* i8259_GetDriver()
{
    return &driver;  
}