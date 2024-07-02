// This software is licensed under the GPL 3.0 license.
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/gpl-3.0.html>.

#pragma once
#include <stdint.h>
#include "tss.h"

#define NULL_ENTRY  0
#define KERNEL_CODE 1
#define KERNEL_DATA 2
#define USER_CODE   3
#define USER_DATA   4
#define TSS         5

struct GdtEntry
{
    uint16_t limit_one;
    uint16_t base_zero;
    uint8_t base_two;
    uint8_t access_byte;
    uint8_t limit_two : 4;
    uint8_t flags : 4;
    uint8_t base_three;
} __attribute__((packed));


struct GdtDescriptor{
    uint16_t Limit;                     // sizeof(Gdt) - 1
    struct GdtEntry* Ptr;
} __attribute__((packed));

typedef enum
{
    GDT_ACCESS_CODE_READABLE                = 0x02,
    GDT_ACCESS_DATA_WRITEABLE               = 0x02,

    GDT_ACCESS_CODE_CONFORMING              = 0x04,
    GDT_ACCESS_DATA_DIRECTION_NORMAL        = 0x00,
    GDT_ACCESS_DATA_DIRECTION_DOWN          = 0x04,

    GDT_ACCESS_DATA_SEGMENT                 = 0x10,
    GDT_ACCESS_CODE_SEGMENT                 = 0x18,
    GDT_ACCESS_DESCRIPTOR_TSS               = 0x00,

    GDT_ACCESS_RING0                        = 0x00,
    GDT_ACCESS_RING1                        = 0x20,
    GDT_ACCESS_RING2                        = 0x40,
    GDT_ACCESS_RING3                        = 0x60,

    GDT_ACCESS_PRESENT                      = 0x80,

} GDT_ACCESS;

typedef enum 
{
    GDT_FLAG_64BIT                          = 0x20,
    GDT_FLAG_32BIT                          = 0x40,
    GDT_FLAG_16BIT                          = 0x00,

    GDT_FLAG_GRANULARITY_1B                 = 0x00,
    GDT_FLAG_GRANULARITY_4K                 = 0x80,
} GDT_FLAGS;

#define GDT_ENTRY(_Base, _Limit, _Access, _Flags) {                    \
    .limit_one = (uint16_t)(_Limit & 0xFFFF),               \
    .base_zero = (uint16_t)(_Base & 0xFFFF),                \
    .base_two = (uint8_t)((_Base & 0xFF0000) >> 16),        \
    .access_byte = (_Access),                                          \
    .limit_two = (uint8_t)((_Limit & 0xF0000) >> 16),       \
    .flags = _Flags,                                                   \
    .base_three = (uint8_t)((_Base & 0xFF000000) >> 24)     \
}     

void InitGDT();