// This software is licensed under the GPL 3.0 license.
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/gpl-3.0.html>.

#include <string.h>
#include "gdt.h"
#define IMPLEMENTATION
#include "gdt_helper.h"

// Define the GDT entries
GdtEntry Gdt[] = {
    GDT_ENTRY(0, 0, 0, 0),               // Null segment
    GDT_ENTRY(0, 0, 0, 0),               // 64-bit kernel code segment
    GDT_ENTRY(0, 0, 0, 0),               // 64-bit kernel data segment
    GDT_ENTRY(0, 0, 0, 0),               // 64-bit user code segment
    GDT_ENTRY(0, 0, 0, 0),               // 64-bit user data segment
    // GDT_ENTRY(0, 0, 0, 0),               // TSS segment (low part)
    // GDT_ENTRY(0, 0, 0, 0)                // TSS segment (high part)
};

static GdtEntry create_gdt_entry(uint64_t descriptor) {
    GdtEntry entry;
    entry.LimitLow = descriptor & 0xFFFF;
    entry.BaseLow = (descriptor >> 16) & 0xFFFF;
    entry.BaseMiddle = (descriptor >> 32) & 0xFF;
    entry.Access = (descriptor >> 40) & 0xFF;
    entry.FlagsLimitHi = (descriptor >> 48) & 0xFF;
    entry.BaseHigh = (descriptor >> 56) & 0xFF;
    return entry;
}

// char buffer[8192] = {0}; // This is in the .data section for the rsp0 of the TSS
// TssEntry tssEntry[1] = {0};

void InitTSS() {
}

extern "C" void loadGDT(uint16_t limit, uint64_t base);

void InitGDT() {
    const GdtEntry null_entry = create_gdt_entry(create_descriptor(0, 0, 0));
    const GdtEntry kernel_code = create_gdt_entry(create_descriptor(0, 0x000FFFFF, (GDT_CODE_PL0)));
    const GdtEntry kernel_data = create_gdt_entry(create_descriptor(0, 0x000FFFFF, (GDT_DATA_PL0)));
    const GdtEntry user_code = create_gdt_entry(create_descriptor(0, 0x000FFFFF, (GDT_CODE_PL3)));
    const GdtEntry user_data = create_gdt_entry(create_descriptor(0, 0x000FFFFF, (GDT_DATA_PL3)));
    Gdt[NULL_ENTRY] = null_entry;
    Gdt[KERNEL_CODE] = kernel_code;
    Gdt[KERNEL_DATA] = kernel_data;
    Gdt[USER_CODE] = user_code;
    Gdt[USER_DATA] = user_data;
    GdtDescriptor descriptor;
    descriptor.Limit = sizeof(Gdt) - 1;
    descriptor.Ptr = Gdt;
    // InitTSS();
    loadGDT(descriptor.Limit, (uint64_t)(uintptr_t)descriptor.Ptr);
    // loadTSS(TSS << 3);
}
