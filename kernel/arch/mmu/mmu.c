/*
 * Copyright (c) - All Rights Reserved.
 * 
 * See the LICENCE file for more information.
 */

#include "mmu.h"
#include <limine.h>
#include <stdio.h>
#include <arch/io/io.h>
#include <stddef.h>
#include <string.h>

#define FRAME_SIZE 4096
static size_t useable_memory = 0, reserved_memory = 0, reclaimable_memory = 0, total_memory = 0;
static size_t limine_memmap_entry_count = 0;
static struct limine_memmap_entry** limine_memmap_entries = NULL;
static struct limine_memmap_response* limine_memmap_response = NULL;
struct limine_memmap_request limine_memmap_request = {
        .id = LIMINE_MEMMAP_REQUEST,
        .revision = 0,
        .response = NULL
};
static uint8_t *bitMapAllocator = NULL;
static size_t bitMapSize = 0, num_frames = 0;

void InitMMU(){
    printf("Initing MMU\n");
    if(limine_memmap_request.response == NULL){
        printf("No memory avaliable\n");
        Panic();
    }
    limine_memmap_entries = limine_memmap_request.response->entries;
    limine_memmap_entry_count = limine_memmap_request.response->entry_count;
    printf("Getting total size\n");
    GetTotalMMSize();
    printf("Setting Bitmap\n");
    SetBitMap();
    printf("Checking memory size\n");
    CheckMemorySize();
    PrintMemoryInfo();
}

void SetBitMap(){
    num_frames = total_memory/FRAME_SIZE;
    bitMapSize = (num_frames+7)/8;
    for (size_t i = 0; i < limine_memmap_entry_count; i++) {
        struct limine_memmap_entry* entry = limine_memmap_entries[i];
        if (entry->type == LIMINE_MEMMAP_USABLE && entry->length >= bitMapSize) {
            bitMapAllocator = (uint8_t *)entry->base;
            entry->base += bitMapSize;
            entry->length -= bitMapSize;
            break;
        }
    }
    if(bitMapAllocator != NULL){
        memset(bitMapAllocator, 0xFF, bitMapSize); // set all entries to used
    }
}

void GetTotalMMSize(){
    for(int i = 0; i < limine_memmap_entry_count; i++){
        struct limine_memmap_entry *entry = limine_memmap_entries[i];
        total_memory += entry->length;
    }
}

void CheckMemorySize(){
    for(int i = 0; i < limine_memmap_entry_count; i++){
        struct limine_memmap_entry *entry = limine_memmap_entries[i];
        printf("{Base: %x Length: %x end: %x type: %d}\n", entry->base, entry->length, entry->base+entry->length, entry->type);
        switch(entry->type){
            case LIMINE_MEMMAP_USABLE:
                useable_memory += entry->length;
                break;
            case LIMINE_MEMMAP_RESERVED:
            case LIMINE_MEMMAP_ACPI_NVS:
            case LIMINE_MEMMAP_BAD_MEMORY:
            case LIMINE_MEMMAP_KERNEL_AND_MODULES:
            case LIMINE_MEMMAP_FRAMEBUFFER:
            case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE:
                reserved_memory += entry->length;
                break;
            case LIMINE_MEMMAP_ACPI_RECLAIMABLE:
                reclaimable_memory += entry->length;
                break;
        }
    }
}

void NotifyReclaimed(enum ReclaimType type, size_t length){}

void PrintMemoryInfo(){
    printf("Memory info\n");
    printf("Useable: %d\n", useable_memory);
    printf("Reserved: %d\n", reserved_memory);
    printf("Reclaimable: %d\n", reclaimable_memory);
    printf("Total: %d\n", total_memory);
    printf("Bitmap\n");
    printf("Start addr: %d\n", bitMapAllocator);
    printf("Size: %d\n", bitMapSize);
    printf("Num frames: %d\n", num_frames);
}

void* kmalloc(size_t size){
    printf("TODO: Kmalloc\n");
    Panic();
}