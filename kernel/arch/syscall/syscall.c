/*
 * Copyright (c) - All Rights Reserved.
 * 
 * See the LICENCE file for more information.
 */

#include "syscall.h"
#include <stdio.h>
#include <stddef.h>
#include <arch/fb/fb.h>
#include <arch/hal/idt/isr.h>
#include <errno.h>
#define MAX_SYSCALLS 255

struct SyscallMeta handlersMetadata[MAX_SYSCALLS];

void InitSyscall(){
    IsrRegisterHandler(0x42, HandleSyscall);
    // TODO: Read syscall table
    // ParseSyscallTable("/boot/syscall.tbl");
}

void MaskSyscall(uint8_t syscall){
    handlersMetadata[syscall].used = 0;
}
void UnMaskSyscall(uint8_t syscall){
    handlersMetadata[syscall].used = 1;
}
void RegisterHandler(uint8_t syscall, SyscallHandler handler){
    handlersMetadata[syscall].handler = handler;
    UnMaskSyscall(syscall);
}

void HandleSyscall(Registers *regs){
    uint64_t syscall = regs->rax;
    if(handlersMetadata[syscall].handler != NULL){
        struct SyscallRegs sysregs = {
            .rbx = regs->rbx,
            .rcx = regs->rcx,
            .rdx = regs->rdx,
            .rdi = regs->rdi,
            .rsi = regs->rsi,
        };
        regs->rax = handlersMetadata[syscall].handler(&sysregs);
    } else{
        // throw a rsod
        printf("No syscall handler in place for syscall %d\n", syscall);
        errno = ENOSYSCALLHANDLE;
        Panic();
    }
}