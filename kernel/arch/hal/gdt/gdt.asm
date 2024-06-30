; gdt.asm

section .data
gdtr:
.limit:
    dw 0 ; limit
.base:
    dq 0 ; base

section .text
global loadGDT
loadGDT:
    mov [gdtr.limit], rdi
    mov [gdtr.base], rsi
    lgdt [gdtr]
    ret

global reloadSegments
reloadSegments:
    push QWORD 0x08
    lea rax, [rel .after]
    push QWORD rax
    retfq
.after:
.reload_data:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret

global loadTSS
loadTSS:
    ; Load Task Register
    mov ax, di
    xor dx, dx
    ltr ax
    ret
