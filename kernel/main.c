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
void KernelInit() {
    HalInitializeModules();
    // bool Installer = CheckInstaller();
    bool Installer = false;
    StartTerminal(Installer);
    // Infinite loop to prevent exit
    while (true) {
        __asm__ volatile("nop");
    }
}
