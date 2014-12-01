#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include "display.h"
#include "interrupts.h"
#include "types.h"

namespace keyboard {

// The physical keys on the keyboard. Characters accessed by holding shift are
// _not_ included. 104-key US layout is assumed.
enum class Key {
  UNKNOWN = 0,

  _1,
  _2,
  _3,
  _4,
  _5,
  _6,
  _7,
  _8,
  _9,
  _0,

  A,
  B,
  C,
  D,
  E,
  F,
  G,
  H,
  I,
  J,
  K,
  L,
  M,
  N,
  O,
  P,
  Q,
  R,
  S,
  T,
  U,
  V,
  W,
  X,
  Y,
  Z,

  TAB,
  SPACE,

  DASH,
  EQUAL,
  BACKSPACE,
  ENTER,
  SEMICOLON,
  SINGLE_QUOTE,
  BACKTICK,
  BACKSLASH,
  COMMA,
  PERIOD,
  SLASH,
  LEFT_SQUARE_BRACKET,
  RIGHT_SQUARE_BRACKET,

  KEYPAD_MULTIPLY,
  KEYPAD_DIVIDE,
  KEYPAD_ADD,
  KEYPAD_SUBTRACT,
  KEYPAD_DECIMAL,
  KEYPAD_ENTER,
  KEYPAD_1,
  KEYPAD_2,
  KEYPAD_3,
  KEYPAD_4,
  KEYPAD_5,
  KEYPAD_6,
  KEYPAD_7,
  KEYPAD_8,
  KEYPAD_9,
  KEYPAD_0,

  F1,
  F2,
  F3,
  F4,
  F5,
  F6,
  F7,
  F8,
  F9,
  F10,
  F11,
  F12,

  CAPS_LOCK,
  NUM_LOCK,
  SCROLL_LOCK,

  ESCAPE,
  PRINT_SCREEN,
  PAUSE,
  MENU,

  LEFT_SHIFT,
  RIGHT_SHIFT,
  LEFT_ALT,
  RIGHT_ALT,
  LEFT_CONTROL,
  RIGHT_CONTROL,
  LEFT_SUPER,
  RIGHT_SUPER,

  HOME,
  END,
  INSERT,
  DELETE,
  PAGE_UP,
  PAGE_DOWN,
  UP,
  DOWN,
  LEFT,
  RIGHT,
};

struct KeyEvent {
  enum Action { UP, DOWN };

  Key key;
  Action action;

  // Modifiers. True if pressed at the time of this event.
  bool shift;
  bool control;
  bool alt;
  bool super;

  // Toggles. True if turned on at the time of this event.
  bool capsLock;
  bool numLock;
  bool scrollLock;
};

const char* keyName(Key key);
void init();
KeyEvent readEvent();
void flushBuffer();

inline void print(keyboard::Key key) {
  display::print(keyName(key));
}

} // namespace keyboard

#endif /* KEYBOARD_H_ */
