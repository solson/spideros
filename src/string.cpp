#include "string.h"

namespace std {

int strlen(const char* str) {
   int i;
   while(str[i])
      i++;
   return i;
}

const char* begin(const char* str) {
  return str;
}

const char* end(const char* str) {
  return str + strlen(str);
}

}

