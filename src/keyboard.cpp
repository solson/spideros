#include "assert.h"
#include "display.h"
#include "keyboard.h"
#include "ports.h"

// TODO: Name the magic numbers in this file.

namespace keyboard {

// Mapping from scancodes to keys (the array is indexed by scancodes).
const Key UNESCAPED_KEYS[] = {
  Key::UNKNOWN,              // 0x00
  Key::ESCAPE,               // 0x01
  Key::_1,                   // 0x02
  Key::_2,                   // 0x03
  Key::_3,                   // 0x04
  Key::_4,                   // 0x05
  Key::_5,                   // 0x06
  Key::_6,                   // 0x07
  Key::_7,                   // 0x08
  Key::_8,                   // 0x09
  Key::_9,                   // 0x0A
  Key::_0,                   // 0x0B
  Key::DASH,                 // 0x0C
  Key::EQUAL,                // 0x0D
  Key::BACKSPACE,            // 0x0E
  Key::TAB,                  // 0x0F
  Key::Q,                    // 0x10
  Key::W,                    // 0x11
  Key::E,                    // 0x12
  Key::R,                    // 0x13
  Key::T,                    // 0x14
  Key::Y,                    // 0x15
  Key::U,                    // 0x16
  Key::I,                    // 0x17
  Key::O,                    // 0x18
  Key::P,                    // 0x19
  Key::LEFT_SQUARE_BRACKET,  // 0x1A
  Key::RIGHT_SQUARE_BRACKET, // 0x1B
  Key::ENTER,                // 0x1C
  Key::LEFT_CONTROL,         // 0x1D
  Key::A,                    // 0x1E
  Key::S,                    // 0x1F
  Key::D,                    // 0x20
  Key::F,                    // 0x21
  Key::G,                    // 0x22
  Key::H,                    // 0x23
  Key::J,                    // 0x24
  Key::K,                    // 0x25
  Key::L,                    // 0x26
  Key::SEMICOLON,            // 0x27
  Key::SINGLE_QUOTE,         // 0x28
  Key::BACKTICK,             // 0x29
  Key::LEFT_SHIFT,           // 0x2A
  Key::BACKSLASH,            // 0x2B
  Key::Z,                    // 0x2C
  Key::X,                    // 0x2D
  Key::C,                    // 0x2E
  Key::V,                    // 0x2F
  Key::B,                    // 0x30
  Key::N,                    // 0x31
  Key::M,                    // 0x32
  Key::COMMA,                // 0x33
  Key::PERIOD,               // 0x34
  Key::SLASH,                // 0x35
  Key::RIGHT_SHIFT,          // 0x36
  Key::KEYPAD_MULTIPLY,      // 0x37
  Key::LEFT_ALT,             // 0x38
  Key::SPACE,                // 0x39
  Key::CAPS_LOCK,            // 0x3A
  Key::F1,                   // 0x3B
  Key::F2,                   // 0x3C
  Key::F3,                   // 0x3D
  Key::F4,                   // 0x3E
  Key::F5,                   // 0x3F
  Key::F6,                   // 0x40
  Key::F7,                   // 0x41
  Key::F8,                   // 0x42
  Key::F9,                   // 0x43
  Key::F10,                  // 0x44
  Key::NUM_LOCK,             // 0x45
  Key::SCROLL_LOCK,          // 0x46
  Key::KEYPAD_7,             // 0x47
  Key::KEYPAD_8,             // 0x48
  Key::KEYPAD_9,             // 0x49
  Key::KEYPAD_SUBTRACT,      // 0x4A
  Key::KEYPAD_4,             // 0x4B
  Key::KEYPAD_5,             // 0x4C
  Key::KEYPAD_6,             // 0x4D
  Key::KEYPAD_ADD,           // 0x4E
  Key::KEYPAD_1,             // 0x4F
  Key::KEYPAD_2,             // 0x50
  Key::KEYPAD_3,             // 0x51
  Key::KEYPAD_0,             // 0x52
  Key::KEYPAD_DECIMAL,       // 0x53
  Key::UNKNOWN,              // 0x54
  Key::UNKNOWN,              // 0x55
  Key::UNKNOWN,              // 0x56
  Key::F11,                  // 0x57
  Key::F12,                  // 0x58
};

// Mapping from scancodes to keys (the array is indexed by scancodes) for
// scancodes which come after the escape scancode.
const Key ESCAPED_KEYS[] = {
  Key::UNKNOWN,       // 0x00
  Key::UNKNOWN,       // 0x01
  Key::UNKNOWN,       // 0x02
  Key::UNKNOWN,       // 0x03
  Key::UNKNOWN,       // 0x04
  Key::UNKNOWN,       // 0x05
  Key::UNKNOWN,       // 0x06
  Key::UNKNOWN,       // 0x07
  Key::UNKNOWN,       // 0x08
  Key::UNKNOWN,       // 0x09
  Key::UNKNOWN,       // 0x0A
  Key::UNKNOWN,       // 0x0B
  Key::UNKNOWN,       // 0x0C
  Key::UNKNOWN,       // 0x0D
  Key::UNKNOWN,       // 0x0E
  Key::UNKNOWN,       // 0x0F
  Key::UNKNOWN,       // 0x10
  Key::UNKNOWN,       // 0x11
  Key::UNKNOWN,       // 0x12
  Key::UNKNOWN,       // 0x13
  Key::UNKNOWN,       // 0x14
  Key::UNKNOWN,       // 0x15
  Key::UNKNOWN,       // 0x16
  Key::UNKNOWN,       // 0x17
  Key::UNKNOWN,       // 0x18
  Key::UNKNOWN,       // 0x19
  Key::UNKNOWN,       // 0x1A
  Key::UNKNOWN,       // 0x1B
  Key::KEYPAD_ENTER,  // 0x1C
  Key::RIGHT_CONTROL, // 0x1D
  Key::UNKNOWN,       // 0x1E
  Key::UNKNOWN,       // 0x1F
  Key::UNKNOWN,       // 0x20
  Key::UNKNOWN,       // 0x21
  Key::UNKNOWN,       // 0x22
  Key::UNKNOWN,       // 0x23
  Key::UNKNOWN,       // 0x24
  Key::UNKNOWN,       // 0x25
  Key::UNKNOWN,       // 0x26
  Key::UNKNOWN,       // 0x27
  Key::UNKNOWN,       // 0x28
  Key::UNKNOWN,       // 0x29
  Key::UNKNOWN,       // 0x2A
  Key::UNKNOWN,       // 0x2B
  Key::UNKNOWN,       // 0x2C
  Key::UNKNOWN,       // 0x2D
  Key::UNKNOWN,       // 0x2E
  Key::UNKNOWN,       // 0x2F
  Key::UNKNOWN,       // 0x30
  Key::UNKNOWN,       // 0x31
  Key::UNKNOWN,       // 0x32
  Key::UNKNOWN,       // 0x33
  Key::UNKNOWN,       // 0x34
  Key::KEYPAD_DIVIDE, // 0x35
  Key::UNKNOWN,       // 0x36
  Key::UNKNOWN,       // 0x37
  Key::RIGHT_ALT,     // 0x38
  Key::UNKNOWN,       // 0x39
  Key::UNKNOWN,       // 0x3A
  Key::UNKNOWN,       // 0x3B
  Key::UNKNOWN,       // 0x3C
  Key::UNKNOWN,       // 0x3D
  Key::UNKNOWN,       // 0x3E
  Key::UNKNOWN,       // 0x3F
  Key::UNKNOWN,       // 0x40
  Key::UNKNOWN,       // 0x41
  Key::UNKNOWN,       // 0x42
  Key::UNKNOWN,       // 0x43
  Key::UNKNOWN,       // 0x44
  Key::UNKNOWN,       // 0x45
  Key::UNKNOWN,       // 0x46
  Key::HOME,          // 0x47
  Key::UP,            // 0x48
  Key::PAGE_UP,       // 0x49
  Key::UNKNOWN,       // 0x4A
  Key::LEFT,          // 0x4B
  Key::UNKNOWN,       // 0x4C
  Key::RIGHT,         // 0x4D
  Key::UNKNOWN,       // 0x4E
  Key::END,           // 0x4F
  Key::DOWN,          // 0x50
  Key::PAGE_DOWN,     // 0x51
  Key::INSERT,        // 0x52
  Key::DELETE,        // 0x53
  Key::UNKNOWN,       // 0x54
  Key::UNKNOWN,       // 0x55
  Key::UNKNOWN,       // 0x56
  Key::UNKNOWN,       // 0x57
  Key::UNKNOWN,       // 0x58
  Key::UNKNOWN,       // 0x59
  Key::UNKNOWN,       // 0x5A
  Key::LEFT_SUPER,    // 0x5B
  Key::RIGHT_SUPER,   // 0x5C
  Key::MENU,          // 0x5D
};

// To simplify the implementation, this ring buffer always keeps one slot
// unused, so only `capacity - 1` items can be stored.
template<typename T, u32 capacity>
class RingQueue {
 public:
  void enqueue(const T& elem) {
    if (isFull()) {
      return;
    }
    data_[end_] = elem;
    end_ = (end_ + 1) % capacity;
  }

  T dequeue() {
    assert(!isEmpty());
    T elem = data_[start_];
    start_ = (start_ + 1) % capacity;
    return elem;
  }

  bool isFull() const {
    return (end_ + 1) % capacity == start_;
  }

  bool isEmpty() const {
    return start_ == end_;
  }
 private:
  u32 start_ = 0;
  u32 end_   = 0;
  T data_[capacity];
};

RingQueue<u8, 256> scancodeQueue;

void init() {
  interrupts::setIrqHandler(1, [](interrupts::Registers*) {
    scancodeQueue.enqueue(ports::inb(0x60));
  });
  flushBuffer();
}

u8 readScancode() {
  // TODO: Find a way to wait that isn't busy-waiting.
  while (scancodeQueue.isEmpty()) {}
  return scancodeQueue.dequeue();
}

// TODO: Handle the special case keys Print Screen and Pause.
KeyEvent readEvent() {
  const int ESCAPE_CODE = 0xE0;
  static bool nextScancodeIsEscaped = false;

  // Persistant storage for the state of the various modifier and toggle keys.
  static bool leftAlt      = false;
  static bool rightAlt     = false;
  static bool leftControl  = false;
  static bool rightControl = false;
  static bool leftShift    = false;
  static bool rightShift   = false;
  static bool leftSuper    = false;
  static bool rightSuper   = false;
  static bool capsLock     = false;
  static bool numLock      = false;
  static bool scrollLock   = false;

  u8 scancode = readScancode();

  if (scancode == ESCAPE_CODE) {
    nextScancodeIsEscaped = true;
    scancode = readScancode();
  }

  KeyEvent event;

  // If the seventh bit is set, the scancode represents a key release, otherwise
  // it's a key press.
  bool isPress = !(scancode & (1 << 7));
  event.action = isPress ? KeyEvent::DOWN : KeyEvent::UP;

  // Clear the seventh bit before indexing into the key code tables below.
  scancode &= ~(1 << 7);

  if (nextScancodeIsEscaped) {
    nextScancodeIsEscaped = false;
    event.key = ESCAPED_KEYS[scancode];
  } else {
    event.key = UNESCAPED_KEYS[scancode];
  }

  // Turn modifiers on or off based on whether this is a press or release.
  switch (event.key) {
    case Key::LEFT_ALT:      leftAlt      = isPress; break;
    case Key::RIGHT_ALT:     rightAlt     = isPress; break;
    case Key::LEFT_CONTROL:  leftControl  = isPress; break;
    case Key::RIGHT_CONTROL: rightControl = isPress; break;
    case Key::LEFT_SHIFT:    leftShift    = isPress; break;
    case Key::RIGHT_SHIFT:   rightShift   = isPress; break;
    case Key::LEFT_SUPER:    leftSuper    = isPress; break;
    case Key::RIGHT_SUPER:   rightSuper   = isPress; break;
    default: break;
  }

  // Switch the toggles if this is a key press, not a release.
  if (isPress) {
    switch (event.key) {
      case Key::CAPS_LOCK:   capsLock   = !capsLock;   break;
      case Key::NUM_LOCK:    numLock    = !numLock;    break;
      case Key::SCROLL_LOCK: scrollLock = !scrollLock; break;
      default: break;
    }
  }

  event.shift      = leftShift   || rightShift;
  event.control    = leftControl || rightControl;
  event.alt        = leftAlt     || rightAlt;
  event.super      = leftSuper   || rightSuper;
  event.capsLock   = capsLock;
  event.numLock    = numLock;
  event.scrollLock = scrollLock;

  if (isPress) {
    if (event.shift) {
      event.character = upperCaseChar(event.key);
    } else {
      event.character = lowerCaseChar(event.key);
    }
  } else {
    event.character = '\0';
  }

  return event;
}

char readChar() {
  KeyEvent event;
  do {
    event = readEvent();
  } while (!event.character);
  return event.character;
}

// Flush the keyboard buffer.
void flushBuffer() {
  while(ports::inb(0x64) & 1) {
    ports::inb(0x60);
  }
}

const char* keyName(Key key) {
  switch (key) {
    case Key::UNKNOWN:              return "<UNKNOWN>";
    case Key::_1:                   return "1";
    case Key::_2:                   return "2";
    case Key::_3:                   return "3";
    case Key::_4:                   return "4";
    case Key::_5:                   return "5";
    case Key::_6:                   return "6";
    case Key::_7:                   return "7";
    case Key::_8:                   return "8";
    case Key::_9:                   return "9";
    case Key::_0:                   return "0";
    case Key::A:                    return "A";
    case Key::B:                    return "B";
    case Key::C:                    return "C";
    case Key::D:                    return "D";
    case Key::E:                    return "E";
    case Key::F:                    return "F";
    case Key::G:                    return "G";
    case Key::H:                    return "H";
    case Key::I:                    return "I";
    case Key::J:                    return "J";
    case Key::K:                    return "K";
    case Key::L:                    return "L";
    case Key::M:                    return "M";
    case Key::N:                    return "N";
    case Key::O:                    return "O";
    case Key::P:                    return "P";
    case Key::Q:                    return "Q";
    case Key::R:                    return "R";
    case Key::S:                    return "S";
    case Key::T:                    return "T";
    case Key::U:                    return "U";
    case Key::V:                    return "V";
    case Key::W:                    return "W";
    case Key::X:                    return "X";
    case Key::Y:                    return "Y";
    case Key::Z:                    return "Z";
    case Key::TAB:                  return "Tab";
    case Key::SPACE:                return "Space";
    case Key::DASH:                 return "-";
    case Key::EQUAL:                return "=";
    case Key::BACKSPACE:            return "Backspace";
    case Key::ENTER:                return "Enter";
    case Key::SEMICOLON:            return ";";
    case Key::SINGLE_QUOTE:         return "'";
    case Key::BACKTICK:             return "`";
    case Key::BACKSLASH:            return "\\";
    case Key::COMMA:                return ",";
    case Key::PERIOD:               return ".";
    case Key::SLASH:                return "/";
    case Key::LEFT_SQUARE_BRACKET:  return "[";
    case Key::RIGHT_SQUARE_BRACKET: return "]";
    case Key::KEYPAD_MULTIPLY:      return "Keypad *";
    case Key::KEYPAD_DIVIDE:        return "Keypad /";
    case Key::KEYPAD_ADD:           return "Keypad +";
    case Key::KEYPAD_SUBTRACT:      return "Keypad -";
    case Key::KEYPAD_DECIMAL:       return "Keypad .";
    case Key::KEYPAD_ENTER:         return "Keypad Enter";
    case Key::KEYPAD_1:             return "Keypad 1";
    case Key::KEYPAD_2:             return "Keypad 2";
    case Key::KEYPAD_3:             return "Keypad 3";
    case Key::KEYPAD_4:             return "Keypad 4";
    case Key::KEYPAD_5:             return "Keypad 5";
    case Key::KEYPAD_6:             return "Keypad 6";
    case Key::KEYPAD_7:             return "Keypad 7";
    case Key::KEYPAD_8:             return "Keypad 8";
    case Key::KEYPAD_9:             return "Keypad 9";
    case Key::KEYPAD_0:             return "Keypad 0";
    case Key::F1:                   return "F1";
    case Key::F2:                   return "F2";
    case Key::F3:                   return "F3";
    case Key::F4:                   return "F4";
    case Key::F5:                   return "F5";
    case Key::F6:                   return "F6";
    case Key::F7:                   return "F7";
    case Key::F8:                   return "F8";
    case Key::F9:                   return "F9";
    case Key::F10:                  return "F10";
    case Key::F11:                  return "F11";
    case Key::F12:                  return "F12";
    case Key::CAPS_LOCK:            return "Caps Lock";
    case Key::NUM_LOCK:             return "Num Lock";
    case Key::SCROLL_LOCK:          return "Scroll Lock";
    case Key::ESCAPE:               return "Escape";
    case Key::PRINT_SCREEN:         return "Print Screen";
    case Key::PAUSE:                return "Pause";
    case Key::MENU:                 return "Menu";
    case Key::LEFT_SHIFT:           return "Left Shift";
    case Key::RIGHT_SHIFT:          return "Right Shift";
    case Key::LEFT_ALT:             return "Left Alt";
    case Key::RIGHT_ALT:            return "Right Alt";
    case Key::LEFT_CONTROL:         return "Left Control";
    case Key::RIGHT_CONTROL:        return "Right Control";
    case Key::LEFT_SUPER:           return "Left Super";
    case Key::RIGHT_SUPER:          return "Right Super";
    case Key::HOME:                 return "Home";
    case Key::END:                  return "End";
    case Key::INSERT:               return "Insert";
    case Key::DELETE:               return "Delete";
    case Key::PAGE_UP:              return "Page Up";
    case Key::PAGE_DOWN:            return "Page Down";
    case Key::UP:                   return "Up";
    case Key::DOWN:                 return "Down";
    case Key::LEFT:                 return "Left";
    case Key::RIGHT:                return "Right";
    default:                        return "<INVALID>";
  }
}

char lowerCaseChar(Key key) {
  switch (key) {
    case Key::_1:                   return '1';
    case Key::_2:                   return '2';
    case Key::_3:                   return '3';
    case Key::_4:                   return '4';
    case Key::_5:                   return '5';
    case Key::_6:                   return '6';
    case Key::_7:                   return '7';
    case Key::_8:                   return '8';
    case Key::_9:                   return '9';
    case Key::_0:                   return '0';
    case Key::A:                    return 'a';
    case Key::B:                    return 'b';
    case Key::C:                    return 'c';
    case Key::D:                    return 'd';
    case Key::E:                    return 'e';
    case Key::F:                    return 'f';
    case Key::G:                    return 'g';
    case Key::H:                    return 'h';
    case Key::I:                    return 'i';
    case Key::J:                    return 'j';
    case Key::K:                    return 'k';
    case Key::L:                    return 'l';
    case Key::M:                    return 'm';
    case Key::N:                    return 'n';
    case Key::O:                    return 'o';
    case Key::P:                    return 'p';
    case Key::Q:                    return 'q';
    case Key::R:                    return 'r';
    case Key::S:                    return 's';
    case Key::T:                    return 't';
    case Key::U:                    return 'u';
    case Key::V:                    return 'v';
    case Key::W:                    return 'w';
    case Key::X:                    return 'x';
    case Key::Y:                    return 'y';
    case Key::Z:                    return 'z';
    case Key::TAB:                  return '\t';
    case Key::SPACE:                return ' ';
    case Key::DASH:                 return '-';
    case Key::EQUAL:                return '=';
    case Key::BACKSPACE:            return '\b';
    case Key::ENTER:                return '\n';
    case Key::SEMICOLON:            return ';';
    case Key::SINGLE_QUOTE:         return '\'';
    case Key::BACKTICK:             return '`';
    case Key::BACKSLASH:            return '\\';
    case Key::COMMA:                return ',';
    case Key::PERIOD:               return '.';
    case Key::SLASH:                return '/';
    case Key::LEFT_SQUARE_BRACKET:  return '[';
    case Key::RIGHT_SQUARE_BRACKET: return ']';
    case Key::KEYPAD_MULTIPLY:      return '*';
    case Key::KEYPAD_DIVIDE:        return '/';
    case Key::KEYPAD_ADD:           return '+';
    case Key::KEYPAD_SUBTRACT:      return '-';
    case Key::KEYPAD_DECIMAL:       return '.';
    case Key::KEYPAD_ENTER:         return '\n';
    case Key::KEYPAD_1:             return '1';
    case Key::KEYPAD_2:             return '2';
    case Key::KEYPAD_3:             return '3';
    case Key::KEYPAD_4:             return '4';
    case Key::KEYPAD_5:             return '5';
    case Key::KEYPAD_6:             return '6';
    case Key::KEYPAD_7:             return '7';
    case Key::KEYPAD_8:             return '8';
    case Key::KEYPAD_9:             return '9';
    case Key::KEYPAD_0:             return '0';
    default:                        return '\0';
  }
}

char upperCaseChar(Key key) {
  switch (key) {
    case Key::_1:                   return '!';
    case Key::_2:                   return '@';
    case Key::_3:                   return '#';
    case Key::_4:                   return '$';
    case Key::_5:                   return '%';
    case Key::_6:                   return '^';
    case Key::_7:                   return '&';
    case Key::_8:                   return '*';
    case Key::_9:                   return '(';
    case Key::_0:                   return ')';
    case Key::A:                    return 'A';
    case Key::B:                    return 'B';
    case Key::C:                    return 'C';
    case Key::D:                    return 'D';
    case Key::E:                    return 'E';
    case Key::F:                    return 'F';
    case Key::G:                    return 'G';
    case Key::H:                    return 'H';
    case Key::I:                    return 'I';
    case Key::J:                    return 'J';
    case Key::K:                    return 'K';
    case Key::L:                    return 'L';
    case Key::M:                    return 'M';
    case Key::N:                    return 'N';
    case Key::O:                    return 'O';
    case Key::P:                    return 'P';
    case Key::Q:                    return 'Q';
    case Key::R:                    return 'R';
    case Key::S:                    return 'S';
    case Key::T:                    return 'T';
    case Key::U:                    return 'U';
    case Key::V:                    return 'V';
    case Key::W:                    return 'W';
    case Key::X:                    return 'X';
    case Key::Y:                    return 'Y';
    case Key::Z:                    return 'Z';
    case Key::TAB:                  return '\t';
    case Key::SPACE:                return ' ';
    case Key::DASH:                 return '_';
    case Key::EQUAL:                return '+';
    case Key::BACKSPACE:            return '\b';
    case Key::ENTER:                return '\n';
    case Key::SEMICOLON:            return ':';
    case Key::SINGLE_QUOTE:         return '"';
    case Key::BACKTICK:             return '~';
    case Key::BACKSLASH:            return '|';
    case Key::COMMA:                return '<';
    case Key::PERIOD:               return '>';
    case Key::SLASH:                return '?';
    case Key::LEFT_SQUARE_BRACKET:  return '{';
    case Key::RIGHT_SQUARE_BRACKET: return '}';
    case Key::KEYPAD_MULTIPLY:      return '*';
    case Key::KEYPAD_DIVIDE:        return '/';
    case Key::KEYPAD_ADD:           return '+';
    case Key::KEYPAD_SUBTRACT:      return '-';
    case Key::KEYPAD_DECIMAL:       return '.';
    case Key::KEYPAD_ENTER:         return '\n';
    case Key::KEYPAD_1:             return '1';
    case Key::KEYPAD_2:             return '2';
    case Key::KEYPAD_3:             return '3';
    case Key::KEYPAD_4:             return '4';
    case Key::KEYPAD_5:             return '5';
    case Key::KEYPAD_6:             return '6';
    case Key::KEYPAD_7:             return '7';
    case Key::KEYPAD_8:             return '8';
    case Key::KEYPAD_9:             return '9';
    case Key::KEYPAD_0:             return '0';
    default:                        return '\0';
  }
}

} // namespace keyboard
