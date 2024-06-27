// This software is licensed under the GPL 3.0 license.
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/gpl-3.0.html>.

#include "isr.h"
#include "idt.h"
#include <stddef.h>
#include <debug.h>

const char* exception_strings[32] = {
        "(#DE) Division Error",
        "(#DB) Debug",
        "(#--) Non-maskable Interrupt",
        "(#BP) Breakpoint",
        "(#OF) Overflow",
        "(#BR) Bound Range Exceeded",
        "(#UD) Invalid Opcode",
        "(#NM) Device Not Available",
        "(#DF) Double Fault",
        "(#--) Coprocessor Segment Overrun",
        "(#TS) Invalid TSS",
        "(#NP) Segment Not Present",
        "(#SS) Stack Segment Fault",
        "(#GP) General Protection Fault",
        "(#PF) Page Fault",
        "(#--) Reserved",
        "(#MF) x87 Floating-Point Exception",
        "(#AC) Alignment Check",
        "(#MC) Machine Check",
        "(#XM) SIMD Floating-Point Exception",
        "(#VE) Virtualization Exception",
        "(#CP) Control Protection Exception",
        "", "", "", "", "", "",
        "(#HV) Hypervisor Injection Exception",
        "(#VC) VMM Communication Exception",
        "(#SX) Security Exception",
        "(#--) Reserved"
};

IsrHandler IsrHandlers[256];

namespace arch{
namespace hal{
namespace idt{
    void InitializeGates();
    void InitISR(){
        InitializeGates();
        for(int i = 0; i < 256; ++i){
            EnableGate(i);
        }
        DisableGate(0x80);
    }

    void Panic(){
        while(1);
    }

    extern "C" void HandleIsr(Registers* regs)
    {
        if (IsrHandlers[regs->interrupt] != nullptr)
            IsrHandlers[regs->interrupt](regs);
        else{
            debug::Log("Unhandled interrupt\n");
            Panic();
        }
    }

    void RegisterHandlerIsr(int interrupt, IsrHandler handler){
        IsrHandlers[interrupt] = handler;
        EnableGate(interrupt);
    }
}
}
}