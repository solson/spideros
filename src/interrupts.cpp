#include "assert.h"
#include "interrupts.h"
#include "display.h"
#include "idt.h"
#include "ports.h"

// TODO: Deal with the magic numbers in this file.

namespace interrupts {

void init() {
  initIsrs();
  initIrqs();
}

template<int n>
[[gnu::naked]] void isr() {
  asm volatile("cli");
  asm volatile("push %0" : : "i"(n));
  asm volatile("jmp isrCommon");
}

template<int n>
[[gnu::naked]] void irq() {
  asm volatile("cli");
  asm volatile("push %0" : : "i"(n));
  asm volatile("jmp irqCommon");
}

void initIsrs() {
  idt::setGate(0,  isr<0>,  0x8, 0, 0, idt::INTR32);
  idt::setGate(1,  isr<1>,  0x8, 0, 0, idt::INTR32);
  idt::setGate(2,  isr<2>,  0x8, 0, 0, idt::INTR32);
  idt::setGate(3,  isr<3>,  0x8, 0, 0, idt::INTR32);
  idt::setGate(4,  isr<4>,  0x8, 0, 0, idt::INTR32);
  idt::setGate(5,  isr<5>,  0x8, 0, 0, idt::INTR32);
  idt::setGate(6,  isr<6>,  0x8, 0, 0, idt::INTR32);
  idt::setGate(7,  isr<7>,  0x8, 0, 0, idt::INTR32);
  idt::setGate(8,  isr<8>,  0x8, 0, 0, idt::INTR32);
  idt::setGate(9,  isr<9>,  0x8, 0, 0, idt::INTR32);
  idt::setGate(10, isr<10>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(11, isr<11>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(12, isr<12>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(13, isr<13>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(14, isr<14>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(15, isr<15>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(16, isr<16>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(17, isr<17>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(18, isr<18>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(19, isr<19>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(20, isr<20>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(21, isr<21>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(22, isr<22>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(23, isr<23>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(24, isr<24>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(25, isr<25>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(26, isr<26>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(27, isr<27>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(28, isr<28>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(29, isr<29>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(30, isr<30>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(31, isr<31>, 0x8, 0, 0, idt::INTR32);
}

void initIrqs() {
  remapPic();

  idt::setGate(32, irq<32>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(33, irq<33>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(34, irq<34>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(35, irq<35>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(36, irq<36>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(37, irq<37>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(38, irq<38>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(39, irq<39>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(40, irq<40>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(41, irq<41>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(42, irq<42>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(43, irq<43>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(44, irq<44>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(45, irq<45>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(46, irq<46>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(47, irq<47>, 0x8, 0, 0, idt::INTR32);
}

// Master PIC command and data port numbers.
const u16 PIC1_COMMAND_PORT = 0x20;
const u16 PIC1_DATA_PORT    = 0x21;

// Slave PIC command and data port numbers.
const u16 PIC2_COMMAND_PORT = 0xA0;
const u16 PIC2_DATA_PORT    = 0xA1;

// End-of-interupt command to send to the PICs when we are finished handling an
// interrupt to resume regularly scheduled programming.
const u8 PIC_EOI = 0x20;

// Normally, IRQs 0 to 7 are mapped to entries 8 to 15. This is a problem in
// protected mode, because IDT entry 8 is a Double Fault. Without remapping,
// every time irq0 fires, you get a Double Fault Exception, which is not
// actually what's happening. We send commands to the Programmable Interrupt
// Controller (PICs, also called the 8259s) in order to remap irq0 to 15 to IDT
// entries 32 to 47.
void remapPic() {
  // Tell the PICs to wait for our 3 initialization bytes (we want to
  // reinitialize).
  ports::outb(PIC1_COMMAND_PORT, 0x11);
  ports::outb(PIC2_COMMAND_PORT, 0x11);

  // Set master PIC offset to 0x20 (= IRQ0 = 32).
  ports::outb(PIC1_DATA_PORT, 0x20);
  // Set slave PIC offset to 0x28 (= IRQ8 = 40).
  ports::outb(PIC2_DATA_PORT, 0x28);

  // Set the wiring to 'attached to corresponding interrupt pin'.
  ports::outb(PIC1_DATA_PORT, 0x04);
  ports::outb(PIC2_DATA_PORT, 0x02);

  // We want to use 8086/8088 mode (bit 0).
  ports::outb(PIC1_DATA_PORT, 0x01);
  ports::outb(PIC2_DATA_PORT, 0x01);

  // Restore masking (if a bit is not set_PORT, the interrupts is on).
  ports::outb(PIC1_DATA_PORT, 0x00);
  ports::outb(PIC2_DATA_PORT, 0x00);
}

IrqHandlerFn irqHandlerFns[16]; // Implicitly zero-initialized.

void setIrqHandler(u32 irqNum, IrqHandlerFn handlerFn) {
  assert(irqNum < 16);
  irqHandlerFns[irqNum] = handlerFn;
}

extern "C" void isrHandler(Registers* regs) {
  display::println("Got isr interrupt: ", regs->interruptNum);

  // Disable interrupts and halt.
  asm volatile("cli; hlt");
}

extern "C" void irqHandler(Registers* regs) {
  const u32 irqNum = regs->interruptNum - 32;
  assert(irqNum < 16);

  if (irqHandlerFns[irqNum]) {
    irqHandlerFns[irqNum](regs);
  } else {
    display::println("Got unhandled irq ", irqNum);
  }

  // We need to send an EOI (end-of-interrupt command) to the interrupt
  // controller when we are done. Only send EOI to slave controller if it's
  // involved (irqs 8 and up).
  if(regs->interruptNum >= 8) {
    ports::outb(PIC2_COMMAND_PORT, PIC_EOI);
  }
  ports::outb(PIC1_COMMAND_PORT, PIC_EOI);
}

} // namespace interrupts
