#include "debug.h"
#include <arch/io/io.h>

void debugLog(const char* fmt){
    while(*fmt){
        outb(0xE9, *fmt);
        fmt++;
    }
}