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

void initIsrs() {
  idt::setGate(0,  isr0,  0x8, 0, 0, idt::INTR32);
  idt::setGate(1,  isr1,  0x8, 0, 0, idt::INTR32);
  idt::setGate(2,  isr2,  0x8, 0, 0, idt::INTR32);
  idt::setGate(3,  isr3,  0x8, 0, 0, idt::INTR32);
  idt::setGate(4,  isr4,  0x8, 0, 0, idt::INTR32);
  idt::setGate(5,  isr5,  0x8, 0, 0, idt::INTR32);
  idt::setGate(6,  isr6,  0x8, 0, 0, idt::INTR32);
  idt::setGate(7,  isr7,  0x8, 0, 0, idt::INTR32);
  idt::setGate(8,  isr8,  0x8, 0, 0, idt::INTR32);
  idt::setGate(9,  isr9,  0x8, 0, 0, idt::INTR32);
  idt::setGate(10, isr10, 0x8, 0, 0, idt::INTR32);
  idt::setGate(11, isr11, 0x8, 0, 0, idt::INTR32);
  idt::setGate(12, isr12, 0x8, 0, 0, idt::INTR32);
  idt::setGate(13, isr13, 0x8, 0, 0, idt::INTR32);
  idt::setGate(14, isr14, 0x8, 0, 0, idt::INTR32);
  idt::setGate(15, isr15, 0x8, 0, 0, idt::INTR32);
  idt::setGate(16, isr16, 0x8, 0, 0, idt::INTR32);
  idt::setGate(17, isr17, 0x8, 0, 0, idt::INTR32);
  idt::setGate(18, isr18, 0x8, 0, 0, idt::INTR32);
  idt::setGate(19, isr19, 0x8, 0, 0, idt::INTR32);
  idt::setGate(20, isr20, 0x8, 0, 0, idt::INTR32);
  idt::setGate(21, isr21, 0x8, 0, 0, idt::INTR32);
  idt::setGate(22, isr22, 0x8, 0, 0, idt::INTR32);
  idt::setGate(23, isr23, 0x8, 0, 0, idt::INTR32);
  idt::setGate(24, isr24, 0x8, 0, 0, idt::INTR32);
  idt::setGate(25, isr25, 0x8, 0, 0, idt::INTR32);
  idt::setGate(26, isr26, 0x8, 0, 0, idt::INTR32);
  idt::setGate(27, isr27, 0x8, 0, 0, idt::INTR32);
  idt::setGate(28, isr28, 0x8, 0, 0, idt::INTR32);
  idt::setGate(29, isr29, 0x8, 0, 0, idt::INTR32);
  idt::setGate(30, isr30, 0x8, 0, 0, idt::INTR32);
  idt::setGate(31, isr31, 0x8, 0, 0, idt::INTR32);
}

void initIrqs() {
  remapPic();

  idt::setGate(32, irq0,  0x8, 0, 0, idt::INTR32);
  idt::setGate(33, irq1,  0x8, 0, 0, idt::INTR32);
  idt::setGate(34, irq2,  0x8, 0, 0, idt::INTR32);
  idt::setGate(35, irq3,  0x8, 0, 0, idt::INTR32);
  idt::setGate(36, irq4,  0x8, 0, 0, idt::INTR32);
  idt::setGate(37, irq5,  0x8, 0, 0, idt::INTR32);
  idt::setGate(38, irq6,  0x8, 0, 0, idt::INTR32);
  idt::setGate(39, irq7,  0x8, 0, 0, idt::INTR32);
  idt::setGate(40, irq8,  0x8, 0, 0, idt::INTR32);
  idt::setGate(41, irq9,  0x8, 0, 0, idt::INTR32);
  idt::setGate(42, irq10, 0x8, 0, 0, idt::INTR32);
  idt::setGate(43, irq11, 0x8, 0, 0, idt::INTR32);
  idt::setGate(44, irq12, 0x8, 0, 0, idt::INTR32);
  idt::setGate(45, irq13, 0x8, 0, 0, idt::INTR32);
  idt::setGate(46, irq14, 0x8, 0, 0, idt::INTR32);
  idt::setGate(47, irq15, 0x8, 0, 0, idt::INTR32);
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
  display::println("Got irq ", irqNum);
  assert(irqNum < 16);

  if (irqHandlerFns[irqNum]) {
    irqHandlerFns[irqNum](regs);
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
