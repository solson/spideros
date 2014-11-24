#include "assert.h"
#include "display.h"

void assertFail(const char* expr, const char* file, const char* line,
                const char* function) {
  display::print("\nAssertion failed at ", file, ':', line, ':', function,
                 ": expression: ", expr, '\n');
}
