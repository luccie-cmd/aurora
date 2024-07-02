// This software is licensed under the GPL 3.0 license.
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/gpl-3.0.html>.

#include "hal.h"
#include "gdt/gdt.h"
#include "idt/idt.h"
#include "idt/isr.h"
#include "irq/irq.h"
#include <arch/fb/fb.h>
#include <arch/io/io.h>
#include <stdio.h>
#include <debug.h>

namespace arch{
namespace hal{
    void Initialize(){
        std::printf("Initializing HAL\n");
        std::printf("Clearing interrupts\n");
        idt::DisableInterrupts();
        std::printf("Initializing GDT\n");
        gdt::InitGDT();
        std::printf("Initializing IDT\n");
        idt::InitIDT();
        std::printf("Initializing frame buffer\n");
        fb::InitFB();
        std::printf("Initializing ISR\n");
        idt::InitISR();
        // std::printf("Initializing IRQ\n");
        // irq::InitIRQ();
        // std::printf("Initializing UEFI\n");
        // uefi::InitUEFI();
        // std::printf("Initializing ACPI\n");
        // acpi::InitACPI();
        // std::printf("Initializing MMU\n");
        // InitMMU();
        // std::printf("Initializing syscall\n");
        // InitSyscall();
        // std::printf("Initializing keyboard driver\n");
        // InitKeyboard();
        std::printf("Hal storing interrupts\n");
        idt::EnableInterrupts();
        std::printf("Hal initialized correctly\n");
    }
}
}