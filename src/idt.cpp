#include "idt.h"
#include "interrupts.h"
#include "util.h"

namespace idt {

Gate idt[IDT_ENTRIES];

void init() {
  memset(idt, 0, sizeof(idt));

  IdtPtr idt_ptr;
  idt_ptr.size = sizeof(idt) - 1;
  idt_ptr.offset = reinterpret_cast<u32>(&idt);

  // Make the CPU load the new IDT.
  asm volatile("lidt (%0)" : : "a"(&idt_ptr));
}

void setGate(u32 n, HandlerFn fn, u16 selector, u8 priv, u8 sys, u8 gateType) {
  u32 offset = reinterpret_cast<u32>(fn);
  idt[n].offsetLow  = offset & 0xffff;       // offset bits 0..15
  idt[n].offsetHigh = offset >> 16 & 0xffff; // offset bits 16..31
  idt[n].selector = selector;
  idt[n].typeAttr = (1 << 7) // First bit must be set for all valid descriptors.
      | ((priv & 0x3) << 5)  // Two bits for the ring level.
      | ((sys  & 0x1) << 4)  // One bit for system segment.
      | (gateType & 0xf);    // Four bits for gate type.
}

} // namespace idt
