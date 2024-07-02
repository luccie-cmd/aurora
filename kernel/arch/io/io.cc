// This software is licensed under the GPL 3.0 license.
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/gpl-3.0.html>.

#include "io.h"
#include <cpuid.h>

namespace arch{
namespace io{
    void outb(uint16_t port, uint8_t value) {
        __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
    }
    bool cpuid(uint32_t function, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t* edx){
        __get_cpuid(function, eax, ebx, ecx, edx);
        return true;
    }
}
}