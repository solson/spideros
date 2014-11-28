#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include "interrupts.h"
#include "types.h"

namespace keyboard {

void init();
void interruptHandler(interrupts::Registers* regs);
void flushBuffer();

} // namespace keyboard

#endif /* KEYBOARD_H_ */
