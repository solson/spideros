#include "display.h"
#include "string.h"

namespace display {

u16 volatile* videoram = (u16 volatile*) 0xb8000;

void clear_screen() {
   for(int i = 0; i < console_height * console_width; i++)
      videoram[i] = (color << 8) | ' ';
}

void put_char_at(char c, int line, int col) {
   videoram[line * console_width + col] = (color << 8) | c;
}

int position = 0;
void print(char c) {
   put_char_at(c, 0, position); // TODO Handle newline, tab, etc.
   position++;
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
