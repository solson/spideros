#include "memory.h"

void memset(void *ptr, i8 val, u32 size) {
    for(unsigned i = 0; i < size; i++)
        static_cast<i8 *>(ptr)[i] = val;
}
