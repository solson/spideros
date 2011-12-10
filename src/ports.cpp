#include "ports.h"

namespace ports {

void outb(u16 port, u8 val) {
    asm volatile("outb %0, %1" : : "a"(val), "d"(port));
}

void outw(u16 port, u16 val) {
    asm volatile("outw %0, %1" : : "a"(val), "d"(port));
}

void outl(u16 port, u32 val) {
    asm volatile("outl %0, %1" : : "a"(val), "d"(port));
}

u8 inb(u16 port) {
    u8 val;
    asm volatile("inb %1, %0" : "=a"(val) : "d"(port));
    return val;
}

u16 inw(u16 port) {
    u16 val;
    asm volatile("inw %1, %0" : "=a"(val) : "d"(port));
    return val;
}


u32 inl(u16 port) {
    u32 val;
    asm volatile("inl %1, %0" : "=a"(val) : "d"(port));
    return val;
}

} // namespace ports

