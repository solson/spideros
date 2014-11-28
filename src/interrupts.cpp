#include "assert.h"
#include "interrupts.h"
#include "display.h"
#include "idt.h"
#include "ports.h"

// TODO: Deal with the magic numbers in this file.

namespace interrupts {

template<int n>
[[gnu::naked]] void interrupt() {
  asm volatile("cli");
  asm volatile("push %0" : : "i"(n));
  asm volatile("jmp interruptCommon");
}

// Set the IDT gates for interrupts from 0 up to N - 1
template<unsigned N>
[[gnu::always_inline]] void setIdtGates() {
  setIdtGates<N - 1>();
  idt::setGate(N - 1,  interrupt<N - 1>,  0x8, 0, 0, idt::INTR32);
}

template<> void setIdtGates<0>() {}

void init() {
  remapPic();
  setIdtGates<48>();
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

extern "C" void interruptHandler(Registers* regs) {
  if (regs->interruptNum >= 32 && regs->interruptNum < 48) {
    const u32 irqNum = regs->interruptNum - 32;

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
  } else {
    display::println("Got isr interrupt: ", regs->interruptNum);

    // Disable interrupts and halt.
    asm volatile("cli; hlt");
  }
}

} // namespace interrupts
