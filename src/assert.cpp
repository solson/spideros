#include "assert.h"
#include "display.h"

void assertFail(const char* expr, const char* file, const char* line,
                const char* function) {
  display::println("\nAssertion failed at {}:{}:{}: expression: {}", file, line,
                   function, expr);
}
