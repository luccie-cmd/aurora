// This software is licensed under the GPL 3.0 license.
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/gpl-3.0.html>.

#pragma once
#include <stdint.h>

typedef struct {
    // In the reverse order they are pushed:
    uint64_t ds;                                            // Data segment pushed by us (though rarely used in 64-bit)
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;          // High registers
    uint64_t rdi, rsi, rbp, rbx, rdx, rcx, rax;             // General-purpose registers
    uint64_t interrupt, error;                              // Interrupt number and error code
    uint64_t rip, cs, rflags, rsp, ss;                      // Pushed automatically by CPU
} __attribute__((packed)) Registers;

typedef void (*IsrHandler)(Registers* regs);

namespace arch{
namespace hal{
namespace idt{
    void InitISR();
    void RegisterHandler(int interrupt, IsrHandler handler);
}
}
}