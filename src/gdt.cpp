#include "gdt.h"
#include "assert.h"
#include "util.h"

// See http://wiki.osdev.org/GDT, or none of this will make any sense

namespace gdt {

Entry gdt[GDT_ENTRIES];
unsigned nextEntry;

void init() {
  // Zero the GDT entries initially since we won't use all of them.
  memset(gdt, 0, sizeof(gdt));

  // Skip the first entry at index 0, it must be left zeroed.
  nextEntry = 1;

  addEntry(0, 0xFFFFF, GDT_PRESENT | GDT_DPL0 | GDT_CODE | GDT_READABLE,
      GDT_GRANULAR | GDT_32BIT);

  addEntry(0, 0xFFFFF, GDT_PRESENT | GDT_DPL0 | GDT_DATA | GDT_WRITABLE,
      GDT_GRANULAR | GDT_32BIT);

  GdtPtr gdt_ptr;
  gdt_ptr.size = sizeof(gdt) - 1;
  gdt_ptr.offset = reinterpret_cast<u32>(&gdt);
  loadGDT(&gdt_ptr);
}

void addEntry(u32 base, u32 limit, u8 accessByte, u8 flags) {
  assert(nextEntry < GDT_ENTRIES);

  gdt[nextEntry].baseLow         = base & 0xFFFF;       // base 0..15
  gdt[nextEntry].baseMid         = base >> 16 & 0xFF;   // base 16..23
  gdt[nextEntry].baseHigh        = base >> 24 & 0xFF;   // base 24..31
  gdt[nextEntry].limitLow        = limit & 0xFFFF;      // limit 0..15
  gdt[nextEntry].flags_limitHigh = flags
                                 | (limit >> 16 & 0xF); // limit 16..19
  gdt[nextEntry].accessByte      = accessByte
                                 | 0x10;                // This bit must be 1.

  nextEntry++;
}

} // namespace gdt
