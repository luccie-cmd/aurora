/*
 * Copyright (c) - All Rights Reserved.
 * 
 * See the LICENCE file for more information.
 */

#pragma once
#include <arch/io/io.h>

struct SyscallRegs{
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rsi;
    uint64_t rdi;
};

typedef uint64_t (*SyscallHandler)(struct SyscallRegs *regs);

enum SyscallPermissions {
    PERMISSION_KERNEL,
    PERMISSION_USER,
    PERMISSION_NONE
};

struct SyscallMeta{
    uint8_t used : 1;
    uint8_t arguments : 3;
    uint8_t permissions : 4;
    SyscallHandler handler;
};


void InitSyscall();
void MaskSyscall(uint8_t syscall);
void UnMaskSyscall(uint8_t syscall);
void RegisterHandler(uint8_t syscall, SyscallHandler handler);
void HandleSyscall(Registers *regs);