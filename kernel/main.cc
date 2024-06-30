// This software is licensed under the GPL 3.0 license.
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/gpl-3.0.html>.

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <arch/hal/hal.h>
#include <arch/fb/fb.h>
#include <debug.h>

void StartTerminal(bool installer){
    (void)installer;
    // TODO: Launch the terminal for now we just halt the OS
    while(true){
        __asm__ volatile("nop");
    }
}

// Kernel initialization function
extern "C" void KernelInit() {
    debug::Log("Booted into AURORA!\n");
    arch::hal::Initialize();
    for(;;);
    // bool Installer = CheckInstaller();
    bool Installer = false;
    StartTerminal(Installer);
    // Infinite loop to prevent exit
    while (true) {
        __asm__ volatile("nop");
    }
}
