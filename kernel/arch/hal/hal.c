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

void HalInitialize(){
    printf("Initializing HAL\n");
    printf("Clearing interrupts\n");
    IdtDisableInterrupts();
    printf("Initializing GDT\n");
    InitGDT();
    printf("Initializing IDT\n");
    InitIDT();
    printf("Initializing frame buffer\n");
    InitFB();
    printf("Initializing ISR\n");
    InitISR();
//     std::printf("Initializing UEFI\n");
//     uefi::InitUEFI();
//     std::printf("Initializing ACPI\n");
//     acpi::InitACPI();
//     std::printf("Initializing MMU\n");
//     mmu::InitMMU();
//     std::printf("Initializing IRQ\n");
//     irq::InitIRQ();
//     std::printf("Initializing syscall\n");
//     InitSyscall();
//     std::printf("Initializing keyboard driver\n");
//     InitKeyboard();
    printf("Hal storing interrupts\n");
    IdtEnableInterrupts();
    printf("Hal initialized correctly\n");
}