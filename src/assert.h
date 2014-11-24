#ifndef ASSERT_H
#define ASSERT_H

// For some reason these macros are what you need to turn __LINE__ into a string
// literal instead of an integer literal.
#define STRING(x) #x
#define MACRO_STRING(x) STRING(x)

// Define NDEBUG to disable assertions.
#ifdef NDEBUG

#define assert(expr)

#else

#define assert(expr)  \
  if (!(expr))        \
    assertFail(#expr, __FILE__, MACRO_STRING(__LINE__), __PRETTY_FUNCTION__);

#endif // NDEBUG

void assertFail(const char* expr, const char* file, const char* line,
  const char* function);

#endif // ASSERT_H
