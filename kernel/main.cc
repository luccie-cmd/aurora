#include <stdint.h>
#include <stddef.h>

// External references to the init array
extern void (*__init_array[])();
extern void (*__init_array_end[])();

// Kernel initialization function
extern "C" void kernel_init() {
    // Iterate over the init array and call each function
    for (size_t i = 0; &__init_array[i] != __init_array_end; i++)
    {
        __init_array[i]();
    }

    // Infinite loop to prevent exit
    while (true) {
        asm volatile("nop");
    }
}
