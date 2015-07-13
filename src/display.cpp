#include "display.h"
#include "ports.h"
#include "assert.h"

namespace display {

Cell* videoram = reinterpret_cast<Cell*>(0xb8000);
int cursorX = 0;
int cursorY = 0;
u8 color;

// Indexes for the index port.
const int CURSOR_LOW_PORT  = 0x0E;
const int CURSOR_HIGH_PORT = 0x0F;

// Display-related I/O ports. They are taken from the BIOS Data Area during
// init();
u16 indexPort;
u16 dataPort;

void init() {
  // Find the base IO port for video from the BIOS Data Area. See
  // http://wiki.osdev.org/Memory_Map_%28x86%29#BIOS_Data_Area_.28BDA.29
  u16 baseIoPort = *reinterpret_cast<u16*>(0x0463);
  indexPort = baseIoPort;
  dataPort  = baseIoPort + 1;

  setColor(Color::LIGHT_GREY, Color::BLACK);
}

void setColor(Color fg, Color bg) {
  color = (u8(bg) << 4) | (u8(fg) & 0x0F);
}

void clearScreen() {
  for (int i = 0; i < CONSOLE_HEIGHT * CONSOLE_WIDTH; i++)
    videoram[i] = Cell(color, ' ');

  cursorX = 0;
  cursorY = 0;
  updateCursor();
}

void scroll() {
  // Copy each line onto the one above it.
  for (int y = 0; y < CONSOLE_HEIGHT - 1; y++)
    for (int x = 0; x < CONSOLE_WIDTH; x++)
      cellAt(x, y) = cellAt(x, y + 1);

  // Blank out the bottom line.
  for (int x = 0; x < CONSOLE_WIDTH; x++)
    cellAt(x, CONSOLE_HEIGHT - 1) = Cell(color, ' ');
}

// Update the position of the blinking cursor on the screen.
void updateCursor() {
  const int position = cursorY * CONSOLE_WIDTH + cursorX;

  ports::outb(indexPort, CURSOR_LOW_PORT);
  ports::outb(dataPort, position >> 8);

  ports::outb(indexPort, CURSOR_HIGH_PORT);
  ports::outb(dataPort, position);
}

Cell& cellAt(int x, int y) {
  return videoram[y * CONSOLE_WIDTH + x];
}

void printAt(char c, int x, int y) {
  cellAt(x, y) = Cell(color, c);
}

void printChar(char c) {
  switch(c) {
    case '\b':
      if (cursorX != 0) {
        cursorX--;
      }
      printAt(' ', cursorX, cursorY);
      break;

    case '\t':
      // Align cursorX to the next multiple of 8
      cursorX = cursorX - (cursorX % 8) + 8;
      break;

    case '\r':
      cursorX = 0;
      break;

    case '\n':
      cursorX = 0;
      cursorY++;
      break;

    default:
      printAt(c, cursorX, cursorY);
      cursorX++;
  }

  if (cursorX == CONSOLE_WIDTH) {
    cursorX = 0;
    cursorY++;
  }

  if (cursorY == CONSOLE_HEIGHT) {
    cursorY--;
    scroll();
  }

  updateCursor();
}

void printString(const char* str) {
  while (*str != '\0') {
    printChar(*str);
    str++;
  }
}

void printInt(u32 n, int radix) {
  assert(radix >= 2 && radix <= 36);

  // Support up to base 36.
  const char numerals[36] = {'0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K',
    'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z'};

  // 32 chars max in the smallest base (binary) for 32-bit integers.
  char buffer[32];

  if (n == 0) {
    printChar('0');
    return;
  }

  // Keep track of how long the number is.
  int i = 0;

  // Fill the buffer with digits from least significant to most
  // significant (reverse digit order).
  while (n != 0) {
    buffer[i] = numerals[n % radix];
    i++;
    n /= radix;
  }

  // Print the buffer in reverse order, since the digits are reversed.
  while (i != 0) {
    printChar(buffer[i - 1]);
    i--;
  }
}

} // namespace display
