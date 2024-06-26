#include "isr.h"
#include "idt.h"
#include <stddef.h>
#include <debug.h>

const char* exception_strings[32] = {
        "(#DE) Division Error",
        "(#DB) Debug",
        "(#--) Non-maskable Interrupt",
        "(#BP) Breakpoint",
        "(#OF) Overflow",
        "(#BR) Bound Range Exceeded",
        "(#UD) Invalid Opcode",
        "(#NM) Device Not Available",
        "(#DF) Double Fault",
        "(#--) Coprocessor Segment Overrun",
        "(#TS) Invalid TSS",
        "(#NP) Segment Not Present",
        "(#SS) Stack Segment Fault",
        "(#GP) General Protection Fault",
        "(#PF) Page Fault",
        "(#--) Reserved",
        "(#MF) x87 Floating-Point Exception",
        "(#AC) Alignment Check",
        "(#MC) Machine Check",
        "(#XM) SIMD Floating-Point Exception",
        "(#VE) Virtualization Exception",
        "(#CP) Control Protection Exception",
        "", "", "", "", "", "",
        "(#HV) Hypervisor Injection Exception",
        "(#VC) VMM Communication Exception",
        "(#SX) Security Exception",
        "(#--) Reserved"
};

IsrHandler IsrHandlers[256];

void InitializeIsr(){
    ISR_InitializeGates();
    for(int i = 0; i < 256; ++i){
        EnableGateIdt(i);
    }
    DisableGateIdt(0x80);
}

void Panic(){
    while(1);
}

void HandleIsr(Registers* regs)
{
    if (IsrHandlers[regs->interrupt] != NULL)
        IsrHandlers[regs->interrupt](regs);
    else{
        debugLog("Unhandled interrupt\n");
        Panic();
    }
}

void RegisterHandlerIsr(int interrupt, IsrHandler handler){
    IsrHandlers[interrupt] = handler;
    EnableGateIdt(interrupt);
}