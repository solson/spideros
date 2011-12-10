#ifndef PORTS_H
#define PORTS_H

namespace ports {

void outb(u16 port, u8  val);
void outw(u16 port, u16 val);
void outl(u16 port, u32 val);

u8  inb(u16 port);
u16 inw(u16 port);
u32 inl(u16 port);

} // namespace ports

#endif // PORTS_H
