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

// TODO: Allow the color to be changed.
const u8 color = 0x07; /* light grey (7) on black (0). */

enum class color : u8 {
    // Bright bit unset
    black = 0,
    blue,
    green,
    cyan,
    red,
    magenta,
    brown,
    lightGrey,

    // Bright bit set
    darkGrey,
    lightBlue,
    lightGreen,
    lightCyan,
    lightRed,
    lightMagenta,
    yellow,
    white
};

void init();
void clearScreen();
void scroll();
void updateCursor();
u16 &cellAt(int x, int y);

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

