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
[[gnu::naked]] void interrupt() {
  asm volatile("cli");
  asm volatile("push %0" : : "i"(n));
  asm volatile("jmp interruptCommon");
}

void initIsrs() {
  idt::setGate(0,  interrupt<0>,  0x8, 0, 0, idt::INTR32);
  idt::setGate(1,  interrupt<1>,  0x8, 0, 0, idt::INTR32);
  idt::setGate(2,  interrupt<2>,  0x8, 0, 0, idt::INTR32);
  idt::setGate(3,  interrupt<3>,  0x8, 0, 0, idt::INTR32);
  idt::setGate(4,  interrupt<4>,  0x8, 0, 0, idt::INTR32);
  idt::setGate(5,  interrupt<5>,  0x8, 0, 0, idt::INTR32);
  idt::setGate(6,  interrupt<6>,  0x8, 0, 0, idt::INTR32);
  idt::setGate(7,  interrupt<7>,  0x8, 0, 0, idt::INTR32);
  idt::setGate(8,  interrupt<8>,  0x8, 0, 0, idt::INTR32);
  idt::setGate(9,  interrupt<9>,  0x8, 0, 0, idt::INTR32);
  idt::setGate(10, interrupt<10>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(11, interrupt<11>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(12, interrupt<12>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(13, interrupt<13>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(14, interrupt<14>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(15, interrupt<15>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(16, interrupt<16>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(17, interrupt<17>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(18, interrupt<18>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(19, interrupt<19>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(20, interrupt<20>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(21, interrupt<21>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(22, interrupt<22>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(23, interrupt<23>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(24, interrupt<24>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(25, interrupt<25>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(26, interrupt<26>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(27, interrupt<27>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(28, interrupt<28>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(29, interrupt<29>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(30, interrupt<30>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(31, interrupt<31>, 0x8, 0, 0, idt::INTR32);
}

void initIrqs() {
  remapPic();

  idt::setGate(32, interrupt<32>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(33, interrupt<33>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(34, interrupt<34>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(35, interrupt<35>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(36, interrupt<36>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(37, interrupt<37>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(38, interrupt<38>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(39, interrupt<39>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(40, interrupt<40>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(41, interrupt<41>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(42, interrupt<42>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(43, interrupt<43>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(44, interrupt<44>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(45, interrupt<45>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(46, interrupt<46>, 0x8, 0, 0, idt::INTR32);
  idt::setGate(47, interrupt<47>, 0x8, 0, 0, idt::INTR32);
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
