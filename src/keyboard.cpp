#include "display.h"
#include "keyboard.h"
#include "ports.h"

// TODO: Name the magic numbers in this file.

namespace keyboard {

void init() {
  interrupts::setIrqHandler(1, interruptHandler);
}

void interruptHandler(interrupts::Registers*) {
  u8 scancode = ports::inb(0x60);

  display::println("Keyboard scancode: 0x", display::hex(scancode), " (",
                   scancode, ")");

  flushBuffer();
}

// Flush the keyboard buffer. This must be done after each key press or the
// keyboard will stop sending interrupts.
void flushBuffer() {
  while(ports::inb(0x64) & 1) {
    ports::inb(0x60);
  }
}

} // namespace keyboard
