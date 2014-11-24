#ifndef GDT_H
#define GDT_H

#include "types.h"

// See http://wiki.osdev.org/GDT, or none of this will make any sense.

namespace gdt {

// Pointer to the Global Descriptor Table for the lgdt operation.
struct [[gnu::packed]] GdtPtr {
  u16 size;
  u32 offset;
};

// Structure for entries in the Global Descriptor Table.
struct [[gnu::packed]] Entry {
  u16 limitLow;       // limit 0..15
  u16 baseLow;        // base 0..15
  u8 baseMid;         // base 16..23
  u8 accessByte;
  u8 flags_limitHigh; // limit 16..19
  u8 baseHigh;        // base 24..31
};

// Number of entries in the GDT.
const unsigned GDT_ENTRIES = 256;

enum {
  // Access byte properties.
  GDT_PRESENT    = 0x80,

  GDT_DPL0       = 0x00, // DPL = descriptor privilege level (ring level).
  GDT_DPL1       = 0x20,
  GDT_DPL2       = 0x40,
  GDT_DPL3       = 0x60,

  GDT_CODE       = 0x08,
  GDT_READABLE   = 0x02,
  GDT_CONFORMING = 0x04,

  GDT_DATA       = 0x00,
  GDT_WRITABLE   = 0x02,
  GDT_GROW_DOWN  = 0x04,

  // Flags
  GDT_GRANULAR   = 0x80,
  GDT_32BIT      = 0x40,
  GDT_16BIT      = 0x00
};

void init();
void addEntry(u32 base, u32 limit, u8 accessByte, u8 flags);
extern "C" void loadGDT(GdtPtr* gp);

} // namespace gdt

#endif // GDT_H
