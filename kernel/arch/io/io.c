// This software is licensed under the GPL 3.0 license.
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/gpl-3.0.html>.

#include "io.h"
#include <cpuid.h>
#include <stdio.h>
#include "gsod/gsod.h"

void outb(uint16_t port, uint8_t value) {
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void outw(uint16_t port, uint16_t val) {
    __asm__ volatile ("outw %0, %1" : : "a"(val), "Nd"(port));
}

uint16_t inw(uint16_t port) {
    uint16_t ret;
    __asm__ volatile ("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void outl(uint16_t port, uint32_t val) {
    __asm__ volatile ("outl %0, %1" : : "a"(val), "Nd"(port));
}

uint32_t inl(uint16_t port) {
    uint32_t ret;
    __asm__ volatile ("inl %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
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
    GSOD();
    while(1);
}

void iowait(){
    outb(0x80, 0);
}
