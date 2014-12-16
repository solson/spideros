#ifndef UTIL_H_
#define UTIL_H_

#include "types.h"

void memset(void* ptr, i8 val, u32 size);

template<typename T>
T min(const T& a, const T& b) {
  return a < b ? a : b;
}

template<typename T>
T max(const T& a, const T& b) {
  return a > b ? a : b;
}

#endif /* UTIL_H_ */
