/* Tell the linker that we want an x86_64 ELF64 output file */
OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)

/* We want the symbol _start to be our entry point */
ENTRY(_start)

SECTIONS
{
    /* We wanna be placed in the topmost 2GiB of the address space, for optimizations */
    /* and because that is what the Limine spec mandates. */
    /* Any address in this region will do, but often 0xffffffff80000000 is chosen as */
    /* that is the beginning of the region. */
    . = 0xffffffff80000000;

    /* .text section */
    .text :
    {
        *(.text .text.*)
    }

    /* .rodata section */
    .rodata :
    {
        *(.rodata .rodata.*)
    }

    /* .data section */
    .data :
    {
        *(.data .data.*)
    }

    /* Dynamic section for relocations, both in its own PHDR and inside data PHDR */
    .dynamic :
    {
        *(.dynamic)
    }

    /* .bss section */
    .bss :
    {
        *(.bss .bss.*)
        *(COMMON)
    }
}
