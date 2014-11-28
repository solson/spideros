#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "types.h"

namespace interrupts {

// Enable interrupts on the CPU.
inline void enable() {
  asm volatile("sti");
}

// Enable interrupts on the CPU.
inline void disable() {
  asm volatile("cli");
}

struct Registers {
  u32 gs, fs, es, ds;                         // Segment registers.
  u32 edi, esi, ebp, esp, ebx, edx, ecx, eax; // From 'pusha' instruction.
  u32 interruptNum;
  u32 eip, cs, eflags, useresp, ss;           // Pushed by CPU on interrupt.
};

using IrqHandlerFn = void (*)(Registers*);
void setIrqHandler(u32 irqNum, IrqHandlerFn handlerFn);

void init();
void remapPic();

extern "C" void interruptHandler(Registers* regs);

} // namespace interrupts

#endif
