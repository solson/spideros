#ifndef DISPLAY_H
#define DISPLAY_H

#include "types.h"

namespace display {

const int console_width = 80;
const int console_height = 25;

// Display-related I/O ports. They are taken from the BIOS Data Area
// during init();
extern int index_port;
extern int data_port;

// Indexes for the index_port
const int cursor_low_port  = 0x0E;
const int cursor_high_port = 0x0F;

extern int cursor_x, cursor_y;

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
    light_grey,

    // Bright bit set
    dark_grey,
    light_blue,
    light_green,
    light_cyan,
    light_red,
    light_magenta,
    yellow,
    white
};

void init();
void clear_screen();
void scroll();
void update_cursor();
u16 &cell_at(int x, int y);
void put_char_at(char c, int line, int col);

// Printing functions
void printInt(u32 n, int radix);
void printAt(char c, int x, int y);
void print(char c);
void print(const char* str);
void print(i32 x);
void print(u32 x);

// Special number base formatting "tag" struct
template<typename Int>
struct int_base_t {
    Int val;
    int base;
    constexpr int_base_t(Int val, int base) : val(val), base(base) {}
};

template<typename Int>
inline constexpr int_base_t<Int> hex(Int x) {
    return int_base_t<Int>(x, 16);
}

template<typename Int>
inline constexpr int_base_t<Int> oct(Int x) {
    return int_base_t<Int>(x, 8);
}

template<typename Int>
inline constexpr int_base_t<Int> bin(Int x) {
    return int_base_t<Int>(x, 2);
}
template<typename Int>
inline void print(int_base_t<Int> x) { printInt(x.val, x.base); }

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

