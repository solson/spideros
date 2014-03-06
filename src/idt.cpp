#include "idt.h"
#include "memory.h"

namespace idt {

Gate idt[IDT_ENTRIES];

void init() {
    memset(idt, 0, sizeof(idt));

    IdtPtr ip;
    ip.size = sizeof(Gate) * IDT_ENTRIES - 1;
    ip.offset = reinterpret_cast<u32>(&idt);

    loadIDT(&ip);
}

// TODO: Change type of offset.
void setGate(u32 n, void* offset, u16 selector, u8 priv, u8 sys, u8 gateType) {
    idt[n].offsetLow  = u32(offset) & 0xffff;       // offset bits 0..15
    idt[n].offsetHigh = u32(offset) >> 16 & 0xffff; // offset bits 16..31

    idt[n].selector = selector;
    idt[n].zero = 0; // Unused bits must be set to zero.

    idt[n].typeAttr = (1 << 7) // First bit must be set for all valid descriptors.
        | ((priv & 0x3) << 5)  // Two bits for the ring level.
        | ((sys  & 0x1) << 4)  // One bit for system segment.
        | (gateType & 0xf);    // Four bits for gate type.
}

}
