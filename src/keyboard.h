#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include "interrupts.h"
#include "types.h"

namespace keyboard {

void init();
void interruptHandler(interrupts::Registers* regs);
void flushBuffer();

}

#endif /* KEYBOARD_H_ */
