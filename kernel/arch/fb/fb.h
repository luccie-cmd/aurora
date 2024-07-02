// This software is licensed under the GPL 3.0 license.
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/gpl-3.0.html>.

#pragma once
#include <stdint.h>
#include <limine.h>
#include <stddef.h>

extern struct limine_framebuffer** _Framebuffers;
extern struct limine_framebuffer* _MainFramebuffer;
extern volatile struct limine_framebuffer_request _FramebuffersRequest;
extern size_t _Fbcount;

namespace arch{
namespace fb{
    void InitFB();
    void putPixel(uint64_t x, uint64_t y, uint8_t r, uint8_t g, uint8_t b);
    void putc(char c);
}
}