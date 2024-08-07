// This software is licensed under the GPL 3.0 license.
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/gpl-3.0.html>.

#include "idt.h"

void loadIDT(IdtDescriptor* descriptor);

IdtEntry idtEntries[IDT_ENTRIES];
IdtDescriptor idtDescriptor;

void IdtDisableGate(int interrupt){
    idtEntries[interrupt].type_attr &= 0x80;
}
void IdtEnableGate(int interrupt){
    idtEntries[interrupt].type_attr |= 0x80;
}
void IdtSetGate(int interrupt, void* base, uint16_t segmentDescriptor, uint8_t flags){
    idtEntries[interrupt].offset_low = (uint64_t)base & 0xFFFF;
    idtEntries[interrupt].selector = segmentDescriptor;
    idtEntries[interrupt].ist = 0;
    idtEntries[interrupt].type_attr = flags;
    idtEntries[interrupt].offset_mid = ((uint64_t)base >> 16) & 0xFFFF;
    idtEntries[interrupt].offset_high = ((uint64_t)base >> 32) & 0xFFFFFFFF;
    idtEntries[interrupt].zero = 0;
}
void InitIDT() {
    idtDescriptor.limit = sizeof(idtEntries) - 1;
    idtDescriptor.base = idtEntries;
    // Load the IDT
    loadIDT(&idtDescriptor);
}
void IdtEnableInterrupts(){
    __asm__("sti\n");
}
void IdtDisableInterrupts(){
    __asm__("cli\n");
}