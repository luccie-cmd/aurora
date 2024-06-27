; gdt.asm
global loadGDT
global loadTSS

section .data
gdtr:
.limit:
    dw 0 ; limit
.base:
    dq 0 ; base

section .text
loadGDT:
    mov   [gdtr.limit], di
    mov   [gdtr.base], rsi
    xor ax, ax
    mov ds, ax
    ; Load GDT pointer
    lgdt [gdtr]
global reloadSegments
reloadSegments:
    ; Reload CS register:
    push DWORD 0x08                  ; Push code segment selector (32 bits)
    lea rax, [rel .reload_CS]
    push rax                   ; Push offset of code segment
    retf                            ; Perform a far return
.reload_CS:
    ; Reload data segment registers
    mov   ax, 0x10 ; 0x10 is a stand-in for your data segment
    mov   ds, ax
    mov   es, ax
    mov   fs, ax
    mov   gs, ax
    mov   ss, ax
    ret

loadTSS:
    ; Load Task Register
    mov ax, di
    ltr ax
    ret
