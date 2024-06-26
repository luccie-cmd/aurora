#pragma once
#include <stdint.h>
#include <limine.h>
#include <stddef.h>

extern struct limine_framebuffer** _Framebuffers;
extern struct limine_framebuffer* _MainFramebuffer;
extern volatile struct limine_framebuffer_request _FramebuffersRequest;
extern size_t _Fbcount;

void FbInit();
void FbPutPixel(uint64_t x, uint64_t y, uint8_t r, uint8_t g, uint8_t b);
void FbPutc(char c);