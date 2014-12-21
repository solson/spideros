#ifndef DISPLAY_H
#define DISPLAY_H

#include "types.h"

namespace display {

const int CONSOLE_WIDTH = 80;
const int CONSOLE_HEIGHT = 25;

extern int cursorX, cursorY;
extern u8 color;

enum class Color : u8 {
  // Bright bit unset.
  BLACK = 0,
  BLUE,
  GREEN,
  CYAN,
  RED,
  MAGENTA,
  BROWN,
  LIGHT_GREY,

  // Bright bit set.
  DARK_GREY,
  LIGHT_BLUE,
  LIGHT_GREEN,
  LIGHT_CYAN,
  LIGHT_RED,
  LIGHT_MAGENTA,
  YELLOW,
  WHITE
};

// A cell in text video memory. Do not reorder the fields.
struct Cell {
  u8 character;
  u8 color;

  Cell(u8 color, u8 character) : character(character), color(color) {}
};

void init();
void clearScreen();
void scroll();
void updateCursor();
Cell& cellAt(int x, int y);
void setColor(Color fg, Color bg);

// Printing functions.
void printInt(u32 n, int radix);
void printAt(char c, int x, int y);
void printChar(char c);
void printString(const char* s);

inline void printFmt(const char* s, const char*) {
  printString(s);
}

inline void printFmt(char c, const char*) {
  printChar(c);
}

inline void printFmt(u32 x, const char* format) {
  int base = 10;
  if (*format == 'x') {
    base = 16;
  }
  printInt(x, base);
}

inline void printFmt(i32 n, const char*) {
  if (n < 0) {
    printChar('-');
    // Use a cast instead of the expression -n, because -n can
    // overflow. E.g. if n == INT_MIN, -n returns a negative, because
    // -INT_MIN == INT_MIN.
    printInt(static_cast<u32>(n), 10);
  } else {
    printInt(n, 10);
  }
}

inline void print(const char* format) {
  printString(format);
}

template<typename First, typename... Rest>
void print(const char* format, First firstArg, Rest... restArgs) {
  const u32 SPECIFIER_MAX_LENGTH = 64;
  char specifier[SPECIFIER_MAX_LENGTH];
  u32 specifierIndex = 0;
  bool parsingSpecifier = false;
  bool escaped = false;

  for (const char* p = format; *p != '\0'; ++p) {
    char c = *p;
    if (escaped) {
      if (c == '\\' || c == '{') {
        printChar(c);
      } else {
        printChar('\\');
        printChar(c);
      }
      escaped = false;
    } else if (c == '{' && !parsingSpecifier) {
      parsingSpecifier = true;
    } else if (parsingSpecifier) {
      if (c == '}') {
        specifier[specifierIndex] = '\0';
        printFmt(firstArg, specifier);
        print(p + 1, restArgs...);
        return;
      } else {
        specifier[specifierIndex] = c;
        ++specifierIndex;
      }
    } else if (c == '\\') {
      escaped = true;
    } else {
      printChar(c);
    }
  }

  if (escaped) {
    printChar('\\');
  } else if (parsingSpecifier) {
    // TODO: Unmatched { hit end of string. Should panic.
  }
}

template<typename... Args>
void println(const char* format, Args... args) {
  print(format, args...);
  printChar('\n');
}

} // namepspace display

#endif // DISPLAY_H
