#include "display.h"
#include "string.h"
#include "ports.h"

namespace display {

u16 volatile* videoram = (u16 volatile*) 0xb8000;
int cursor_x = 0;
int cursor_y = 0;

void clear_screen() {
    for(int i = 0; i < console_height * console_width; i++)
        videoram[i] = (color << 8) | ' ';

    cursor_x = 0;
    cursor_y = 0;

    update_cursor();
}

void scroll() {
    // Copy each line onto the one above it.
    for(int y = 0; y < console_height - 1; y++) {
        for(int x = 0; x < console_width; x++) {
            const int target_i = y * console_width + x;
            const int source_i = (y + 1) * console_width + x;
            videoram[target_i] = videoram[source_i];
        }
    }

    // Blank out the bottom line.
    for(int x = 0; x < console_width; x++)
        videoram[(console_height - 1) * console_width + x]
            = (color << 8) | ' ';
}

// Update the position of the blinking cursor on the screen.
void update_cursor() {
    const int position = cursor_y * console_width + cursor_x;

    ports::outb(index_port, cursor_low_port);
    ports::outb(data_port, position >> 8);

    ports::outb(index_port, cursor_high_port);
    ports::outb(data_port, position);
}

void put_char_at(char c, int x, int y) {
    videoram[y * console_width + x] = (color << 8) | c;
}

void print(char c) {
    switch(c) {
        case '\b':
            // TODO: Backspace to previous line (except at y=0) and
            // print a space over the backspaced character.
            if(cursor_x != 0)
                cursor_x--;
            break;
        case '\t':
            // Align cursor_x to the next multiple of 8
            cursor_x = cursor_x - (cursor_x % 8) + 8;
            break;
        case '\r':
            cursor_x = 0;
            break;
        case '\n':
            cursor_x = 0;
            cursor_y++;
            break;
        default:
            put_char_at(c, cursor_x, cursor_y);
            cursor_x++;
    }

    if(cursor_x == console_width) {
        cursor_x = 0;
        cursor_y++;
    }

    if(cursor_y == console_height) {
        cursor_y--;
        scroll();
    }

    update_cursor();
}

void print(const char* str) {
    for(char c : str)
        print(c);
}

void printInt(u32 n, int radix) {
    assert(radix >= 2 && radix <= 36);

    // Support up to base 36.
    const char numerals[36] = {'0', '1', '2', '3', '4', '5', '6', '7',
        '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K',
        'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
        'Y', 'Z'};

    // 32 chars max in the smallest base (binary) for 32-bit integers.
    static char buffer[32];

    if(n == 0) {
        print('0');
        return;
    }

    // Keep track of how long the number is.
    int i = 0;

    // Fill the buffer with digits from least significant to most
    // significant (reverse digit order).
    while(n != 0) {
        buffer[i] = numerals[n % radix];
        i++;
        n /= radix;
    }

    // Print the buffer in reverse order, since the digits are reversed.
    while (i != 0) {
        print(buffer[i - 1]);
        i--;
    }
}

void print(u32 n) {
    printInt(n, 10);
}

void print(i32 n) {
    if(n < 0) {
        print('-');
        // Use a cast instead of the expression -n, because -n can
        // overflow. E.g. if n == INT_MIN, -n returns a negative, because
        // -INT_MIN == INT_MIN.
        printInt(static_cast<u32>(n), 10);
    } else {
        printInt(n, 10);
    }
}

} // namespace display
