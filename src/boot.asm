global boot                             ; making entry point visible to linker

extern kmain                            ; kmain is defined in kmain.cpp
extern __cxa_finalize

; Defined by the linker (see linker.ld). Contain the addresses of the start and
; end of the array of function pointers for static object constructors.
extern linker_constructorsStart
extern linker_constructorsEnd

; setting up the Multiboot header - see GRUB docs for details
MODULEALIGN equ  1<<0                   ; align loaded modules on page boundaries
MEMINFO     equ  1<<1                   ; provide memory map
FLAGS       equ  MODULEALIGN | MEMINFO  ; this is the Multiboot 'flag' field
MAGIC       equ    0x1BADB002           ; 'magic number' lets bootloader find the header
CHECKSUM    equ -(MAGIC + FLAGS)        ; checksum required

section .text

align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

; reserve initial kernel stack space
STACKSIZE equ 0x4000                    ; that's 16k.

boot:
    mov  esp, stack + STACKSIZE         ; set up the stack
    push eax                            ; Multiboot magic number
    push ebx                            ; Multiboot info structure

    mov  ebx, linker_constructorsStart  ; call the constructors
    jmp  .ctors_until_end
.call_constructor:
    call [ebx]
    add  ebx,4
.ctors_until_end:
    cmp  ebx, linker_constructorsEnd
    jb   .call_constructor

    call kmain                          ; call kernel proper

    sub esp, 4                          ; call the destructors
    mov [esp], dword 0x0
    call __cxa_finalize                 ; i.e. __cxa_finalize(nullptr)
    add esp, 4

    cli                                 ; disable interrupts
.hang:
    hlt                                 ; halt machine should kernel return
    jmp  .hang

section .bss

align 4
stack:
    resb STACKSIZE                      ; reserve 16k stack on a doubleword boundary
