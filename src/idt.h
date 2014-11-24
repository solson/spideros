#ifndef IDT_H
#define IDT_H

#include "types.h"

// See http://wiki.osdev.org/IDT, or none of this will make any sense

namespace idt {

// Pointer to the Interrupt Descriptor Table for the lidt operation
struct [[gnu::packed]] IdtPtr {
  u16 size;
  u32 offset;
};

// Structure for entries in the Interrupt Descriptor Table
struct [[gnu::packed]] Gate {
  u16 offsetLow;  // offset 0..15
  u16 selector;   // code segment selector in GDT
  u8 zero = 0;    // unused, must be set to 0
  u8 typeAttr;    // type and attributes
  u16 offsetHigh; // offset 16..31
};

// Number of entries in the IDT.
const unsigned IDT_ENTRIES = 256;

using HandlerFn = void (*)();

enum {
  TASK   = 0x5,
  INTR16 = 0x6,
  INTR32 = 0xe,
  TRAP16 = 0x7,
  TRAP32 = 0xf
};

void init();
void setGate(u32 n, HandlerFn fn, u16 selector, u8 priv, u8 sys, u8 gateType);

} // namespace idt

#endif
