#include "gdt.h"
#include "memory.h"

// See http://wiki.osdev.org/GDT, or none of this will make any sense

namespace gdt {

entry gdt[GDT_ENTRIES];
unsigned next_entry;

void init() {
    // Zero the GDT entries initially since we won't use all of them.
    memset(gdt, 0, sizeof(gdt));

    // Skip the first entry at index 0, it must be left zeroed.
    next_entry = 1;

    new_entry(0, 0xFFFFF,
            GDT_PRESENT | GDT_DPL0 | GDT_CODE | GDT_READABLE,
            GDT_GRANULAR | GDT_32BIT);

    new_entry(0, 0xFFFFF,
            GDT_PRESENT | GDT_DPL0 | GDT_DATA | GDT_WRITABLE,
            GDT_GRANULAR | GDT_32BIT);

    gdt_ptr gp;
    gp.size = sizeof(entry) * GDT_ENTRIES - 1;
    gp.offset = reinterpret_cast<u32>(&gdt);

    load_gdt(&gp);
}

void new_entry(u32 base, u32 limit, u8 access_byte, u8 flags) {
    assert(next_entry < GDT_ENTRIES);

    gdt[next_entry].base0_15  = base & 0xFFFF;
    gdt[next_entry].base16_23 = base >> 16 & 0xFF;
    gdt[next_entry].base24_31 = base >> 24 & 0xFF;
    gdt[next_entry].limit0_15 = limit & 0xFFFF;
    gdt[next_entry].flags_limit16_19 = flags | (limit >> 16 & 0xF);
    gdt[next_entry].access_byte = access_byte | 0x10;

    next_entry++;
}

}
