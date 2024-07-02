// This software is licensed under the GPL 3.0 license.
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/gpl-3.0.html>.

#include <string.h>
#include "gdt.h"
#define IMPLEMENTATION
#include "gdt_helper.h"
#include <debug.h>

// Define the GDT entries
struct GdtEntry Gdt[] = {
    GDT_ENTRY(0, 0, 0, 0),               // Null segment
    GDT_ENTRY(0, 0, 0x9A, 0xA),     // 64-bit kernel code segment
    GDT_ENTRY(0, 0, 0x92, 0xC),     // 64-bit kernel data segment
    GDT_ENTRY(0, 0, 0xFA, 0xA),            // 64-bit user code segment
    GDT_ENTRY(0, 0, 0xF2, 0xC),            // 64-bit user data segment
};

void loadGDT(uint16_t limit, struct GdtEntry* base);
void reloadSegments();

void InitGDT() {
    loadGDT(sizeof(Gdt)-1, Gdt);
    reloadSegments();
}