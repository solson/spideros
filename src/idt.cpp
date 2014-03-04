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

}
