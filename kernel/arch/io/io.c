// This software is licensed under the GPL 3.0 license.
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/gpl-3.0.html>.

#include "io.h"
#include <cpuid.h>
#include <stdio.h>

void outb(uint16_t port, uint8_t value) {
    __asm__ volatile ("out %1, %0" : : "a"(value), "Nd"(port));
}
bool cpuid(uint32_t function, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t* edx){
    __cpuid(function, eax, ebx, ecx, edx);
    return true;
}

void write_msr(uint32_t msr, uint64_t value) {
    __asm__ __volatile__("wrmsr" : : "c" (msr), "A" (value));
}

uint64_t read_msr(uint32_t msr) {
    uint64_t value;
    __asm__ __volatile__("rdmsr" : "=A" (value) : "c" (msr));
    return value;
}

void Panic(){
    printf("TODO: RSOD\n");
    while(1);
}

void iowait(){
    outb(0x80, 0);
}

uint8_t inb(uint16_t port){
    uint8_t value;
    __asm__ volatile ("in %0, %1" : "=a"(value) : "Nd"(port));
    return value;
}