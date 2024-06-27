// This software is licensed under the GPL 3.0 license.
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/gpl-3.0.html>.

#include "hal.h"
#include "gdt/gdt.h"
#include "idt/idt.h"
#include "idt/isr.h"
#include <debug.h>

namespace arch{
namespace hal{
    void Initialize(){
        debug::Log("Initializing HAL\n");
        debug::Log("Clearing interrupts\n");
        idt::DisableInterrupts();
        debug::Log("Initializing GDT\n");
        gdt::InitGDT();
        debug::Log("Initializing IDT\n");
        idt::InitIDT();
        debug::Log("Initializing ISR\n");
        idt::InitISR();
        debug::Log("Hal initialized correctly\n");
        // InitIRQ();
        // InitFB();
        // InitMMU();
        // InitSyscall();
        // InitKeyboard();
        debug::Log("Hal done storing interrupts\n");
        idt::EnableInterrupts();
    }
}
}