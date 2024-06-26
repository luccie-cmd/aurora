#pragma once
#include <stdint.h>
#include <stdarg.h>

int vprintf(const char* fmt, va_list args);
int putc(char c);
int puts(const char* str);
int printf(const char* fmt, ...);
void print_buffer(const char* msg, const void* buffer, uint32_t count);