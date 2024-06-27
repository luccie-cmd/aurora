// This software is licensed under the GPL 3.0 license.
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/gpl-3.0.html>.

#include "debug.h"
#include <arch/io/io.h>

namespace debug{
    void Log(const char* fmt){
        while(*fmt){
            arch::io::outb(0xE9, *fmt);
            fmt++;
        }
    }
}