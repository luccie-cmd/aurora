/*
 * Copyright (c) - All Rights Reserved.
 * 
 * See the LICENCE file for more information.
 */

#include "irq.h"
#include <stdint.h>
#include <debug.h>
#include <stdio.h>
#include <arch/io/io.h>
#include "apic.h"
#include <arch/hal/idt/isr.h>
#include "i8259.h"
#include <stddef.h>

static const struct PicDriver* driver = NULL;
IRQHandler IrqHandler[16];
void HandleIRQ(Registers* regs)
{
    int irq = regs->interrupt - 0x20;
    
    if (IrqHandler[irq] != NULL)
    {
        // handle IRQ
        IrqHandler[irq](regs);
    }
    else
    {
        printf("Unhandled IRQ %d", irq);
    }

    // send EOI
    driver->SendEndOfInterrupt(irq);
}

void InitIRQ(){
    const struct PicDriver *picDrivers[] = {
        i8259_GetDriver(),
        apic_GetDriver(),
    };

    for(int i = 0; i < sizeof(picDrivers)/sizeof(picDrivers[0]); ++i){
        if(picDrivers[i]->Probe()){
            driver = picDrivers[i];
            break;
        }
    }
    if(driver == NULL){
        printf("ERROR: No PIC found\n");
        Panic();
    }
    printf("Initializing %s\n", driver->Name);
    driver->Initialize(0x20, 0x28, false);
    // register ISR handlers for each of the 16 irq lines
    for (int i = 0; i < 16; i++)
        IsrRegisterHandler(32+i, HandleIRQ);
}