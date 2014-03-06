bits 32

global loadGDT

section .text

;;; void loadGDT(gdt::GdtPtr*)
loadGDT:
    push  eax
    mov   eax, [esp+0x8]
    lgdt  [eax]
    pop   eax
    ;; Reload CS register containing code selector.
    ;; We can't directly alter CS, so we far jump to change it
    jmp   0x08:.reload_CS ; 0x08 points at the new code selector (2nd in our GDT)
.reload_CS:
    ;; Reload data segment registers.
    mov   ax, 0x10 ; 0x10 points at the new data selector (3rd in our GDT)
    mov   ds, ax
    mov   es, ax
    mov   fs, ax
    mov   gs, ax
    mov   ss, ax
    ret

;;;
;;; Exceptions, ISRs, IRQs, Syscalls
;;;

extern isrHandler
extern irqHandler

global isrSyscall

isrSyscall:
    cli
    push  0x80
    jmp   isrCommon
    iret

%macro HANDLER_COMMON 1
%1Common:
    pusha
    push  ds
    push  es
    push  fs
    push  gs
    mov   ax, 0x10
    mov   ds, ax
    mov   es, ax
    mov   fs, ax
    mov   gs, ax
    mov   eax, esp
    push  eax
    call %1Handler
    pop   eax
    pop   gs
    pop   fs
    pop   es
    pop   ds
    popa
    add   esp, 4
    iret
%endmacro

HANDLER_COMMON isr
HANDLER_COMMON irq

;; WRAPPER (isr|irq), n, intnum
;;   Makes a wrapper for interrupt 'intnum' called isrn or irqn. The wrapper
;;   jumps to isrHandler or irqHandler.
%macro WRAPPER 3
global %1%2
%1%2:
    cli
    push  %3 ; Interrupt number
    jmp   %1Common
%endmacro

;; Wrappers for ISRs 0 through 31 on interrupts 0 to 31.
%assign i 0
%rep 32
    WRAPPER isr, i, i
    %assign i i+1
%endrep

;; Wrappers for IRQs 0 through 15 on interrupts 32 through 47.
%assign i 0
%rep 16
    %assign k i+32
    WRAPPER irq, i, k
    %assign i i+1
%endrep
