#ifndef DISPLAY_H
#define DISPLAY_H

namespace display {

const int console_width = 80;
const int console_height = 25;
const u8 color = 0x07; /* light grey (7) on black (0). */

enum class color : u8 {
    // Bright bit unset
    black = 0, blue, green, cyan,
    red, magenta, brown, light_grey,
    // Bright bit set
    dark_grey, light_blue, light_green, light_cyan,
    light_red, light_magenta, yellow, white
};

void clear_screen();
void scroll();
void put_char_at(char c, int line, int col);

void print(char c);
void print(const char* str);
void print(int x);

// No-op base case for the variadic template print function
inline void print() {}

template<typename First, typename Second, typename... Rest>
inline void print(First first, Second second, Rest... rest) {
    print(first);
    print(second);
    print(rest...);
}

template<typename... Args>
inline void println(Args... args) {
    print(args..., '\n');
}

} // namepspace display

#endif /* DISPLAY_H */

