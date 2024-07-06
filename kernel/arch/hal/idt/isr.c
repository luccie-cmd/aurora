// This software is licensed under the GPL 3.0 license.
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/gpl-3.0.html>.

#include "isr.h"
#include "idt.h"
#include <stddef.h>
#include <debug.h>
#include <arch/io/io.h>
#include <errno.h>
#include <stdio.h>

char* exception_strings[32] = {
        "(#DE) Division Error\n",
        "(#DB) Debug\n",
        "(#--) Non-maskable Interrupt\n",
        "(#BP) Breakpoint\n",
        "(#OF) Overflow\n",
        "(#BR) Bound Range Exceeded\n",
        "(#UD) Invalid Opcode\n",
        "(#NM) Device Not Available\n",
        "(#DF) Double Fault\n",
        "(#--) Coprocessor Segment Overrun\n",
        "(#TS) Invalid TSS\n",
        "(#NP) Segment Not Present\n",
        "(#SS) Stack Segment Fault\n",
        "(#GP) General Protection Fault\n",
        "(#PF) Page Fault\n",
        "(#--) Reserved\n",
        "(#MF) x87 Floating-Point Exception\n",
        "(#AC) Alignment Check\n",
        "(#MC) Machine Check\n",
        "(#XM) SIMD Floating-Point Exceptio\nn",
        "(#VE) Virtualization Exception\n",
        "(#CP) Control Protection Exception\n",
        "", "", "", "", "", "",
        "(#HV) Hypervisor Injection Exception\n",
        "(#VC) VMM Communication Exception\n",
        "(#SX) Security Exception\n",
        "(#--) Reserved\n"
};

IsrHandler IsrHandlers[256];

void segFault(Registers* regs){
    errno = EEXCEPTION;
    errnoInfo = exception_strings[regs->interrupt];
    Panic();

}

void InitializeGates();
void InitISR(){
    InitializeGates();
    for(int i = 0; i < 256; ++i){
        IdtEnableGate(i);
    }
    for(int i = 0; i < 32; ++i){
        IsrRegisterHandler(i, segFault);
    }
}
void HandleIsr(Registers* regs)
{
    if (IsrHandlers[regs->interrupt] != NULL)
        IsrHandlers[regs->interrupt](regs);
    else{
        errno = EUNHANDLEDINT;
        Panic();
    }
}
void IsrRegisterHandler(int interrupt, IsrHandler handler){
    IsrHandlers[interrupt] = handler;
    IdtEnableGate(interrupt);
}