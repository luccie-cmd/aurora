; idt_load.asm

section .text
global loadIDT

loadIDT:
    mov rax, rdi
    lidt [rax]
    ret
