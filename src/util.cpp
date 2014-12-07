#include "util.h"

void memset(void* ptr, i8 val, u32 size) {
  i8* p = static_cast<i8*>(ptr);
  for (u32 i = 0; i < size; i++) {
    p[i] = val;
  }
}
