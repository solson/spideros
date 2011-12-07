#ifndef DISPLAY_H
#define DISPLAY_H

void clear_screen();
void put_char_at(char c, int line, int col);
void put_char(char c);
void put_string(const char* str);

#endif /* DISPLAY_H */

