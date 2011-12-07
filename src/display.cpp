#include "string.h"

const int console_width = 80;
const int console_height = 25;
const u8 color = 0x07; /* light grey (7) on black (0). */

u16 volatile* videoram = (u16 volatile*) 0xb8000;

void clear_screen() {
   for(int i = 0; i < console_height * console_width; i++)
      videoram[i] = (color << 8) | ' ';
}

void put_char_at(char c, int line, int col) {
   videoram[line * console_width + col] = (color << 8) | c;
}

int position = 0;
void put_char(char c) {
   put_char_at(c, 0, position); // TODO Handle newline, tab, etc.
   position++;
}

void put_string(const char* str) {
   for(char c : str)
      put_char(c);
}

