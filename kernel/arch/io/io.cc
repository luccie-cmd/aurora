// This software is licensed under the GPL 3.0 license.
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/gpl-3.0.html>.

#include "io.h"

namespace arch{
namespace io{
    void outb(uint16_t port, uint8_t value) {
        __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
    }
}
}