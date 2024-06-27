// This software is licensed under the GPL 3.0 license.
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/gpl-3.0.html>.

#pragma once
#include <stdint.h>
#include <stdarg.h>

namespace std{
    int vprintf(const char* fmt, va_list args);
    int putc(char c);
    int puts(const char* str);
    int printf(const char* fmt, ...);
    void print_buffer(const char* msg, const void* buffer, uint32_t count);
}