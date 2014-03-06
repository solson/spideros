#include "interrupts.h"
#include "display.h"

namespace interrupts {

extern "C" void isrHandler(Registers* regs) {
    display::println("Got (isr) interrupt: ", regs->interruptNum);
    asm("cli; hlt");
}

extern "C" void irqHandler(Registers* regs) {
    display::println("Got (irq) interrupt: ", regs->interruptNum);
    asm("cli; hlt");
}

} // namespace interrupts
