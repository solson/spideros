#ifndef DISPLAY_H
#define DISPLAY_H

#include "types.h"

namespace display {

const int consoleWidth = 80;
const int consoleHeight = 25;

// Display-related I/O ports. They are taken from the BIOS Data Area
// during init();
extern int indexPort;
extern int dataPort;

// Indexes for the index_port
const int cursorLowPort  = 0x0E;
const int cursorHighPort = 0x0F;

extern int cursorX, cursorY;
extern u8 color;

enum class Color : u8 {
    // Bright bit unset
    BLACK = 0,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    LIGHT_GREY,

    // Bright bit set
    DARK_GREY,
    LIGHT_BLUE,
    LIGHT_GREEN,
    LIGHT_CYAN,
    LIGHT_RED,
    LIGHT_MAGENTA,
    YELLOW,
    WHITE
};

void init();
void clearScreen();
void scroll();
void updateCursor();
u16& cellAt(int x, int y);
void setColor(Color fg, Color bg);

// Printing functions
void printInt(u32 n, int radix);
void printAt(char c, int x, int y);
void print(char c);
void print(const char* str);
void print(i32 x);
void print(u32 x);

// Special number base formatting "tag" struct
template<typename Int>
struct IntBase {
    Int val;
    int base;
    constexpr IntBase(Int val, int base) : val(val), base(base) {}
};

template<typename Int>
inline constexpr IntBase<Int> hex(Int x) {
    return IntBase<Int>(x, 16);
}

template<typename Int>
inline constexpr IntBase<Int> oct(Int x) {
    return IntBase<Int>(x, 8);
}

template<typename Int>
inline constexpr IntBase<Int> bin(Int x) {
    return IntBase<Int>(x, 2);
}

template<typename Int>
inline void print(IntBase<Int> x) {
    printInt(x.val, x.base);
}

// No-op base case for the variadic template print function
inline void print() {}

// Variadic print function which calls print(arg) for each argument it gets.
template<typename First, typename Second, typename... Rest>
inline void print(First first, Second second, Rest... rest) {
    print(first);
    print(second);
    print(rest...);
}

// Variadic print function that adds a newline at the end.
template<typename... Args>
inline void println(Args... args) {
    print(args..., '\n');
}

} // namepspace display

#endif /* DISPLAY_H */

