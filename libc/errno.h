/*
 * Copyright (c) - All Rights Reserved.
 * 
 * See the LICENCE file for more information.
 */

#pragma once
#include <stdint.h>

enum errnoErrors {
    EGOOD,
    ENODISKS,
    EINVAL,
    ENOSYSCALLHANDLE,
    ETODO,
    ENOMEMORY,
    EEXCEPTION,
    EUNHANDLEDINT,
    EMANYDISKS,
    ENOHHDM,
    MAX_ERRNO,
};

typedef uint64_t errnum;
extern errnum errno;
extern char* errnoInfo;