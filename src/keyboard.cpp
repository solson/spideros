#include "keyboard.h"
#include "ports.h"

namespace keyboard {

void init() {
  interrupts::setIrqHandler(1, interruptHandler);
}

void interruptHandler(interrupts::Registers*) {
  flushBuffer();
}

// Flush the keyboard buffer. This must be done after each key press or the
// keyboard will stop sending interrupts.
void flushBuffer() {
  // TODO: Name these magic numbers.
  while(ports::inb(0x64) & 1) {
    ports::inb(0x60);
  }
}

}
