; gdt.asm
global loadGDT
global loadTSS

section .data
gdtr:
    dw 0 ; limit
    dq 0 ; base

section .text
loadGDT:
    mov   [gdtr], di
    mov   [gdtr+2], rsi
    ; Load GDT pointer
    lgdt [gdtr]
reloadSegments:
    ; Reload CS register:
    push 0x08                 ; Push code segment to stack, 0x08 is a stand-in for your code segment
    lea rax, [rel .reload_CS] ; Load address of .reload_CS into rax
    push rax                  ; Push this value to the stack
    retfq                     ; Perform a far return, RETFQ or LRETQ depending on syntax
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
