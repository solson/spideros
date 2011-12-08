#include "string.h"

namespace std {

int strlen(const char* str) {
    int i;
    while(str[i] != '\0')
        i++;
    return i;
}

const char* begin(const char* str) {
    return str;
}

const char* end(const char* str) {
    // Find the end of the string (where the nul is found)
    while(*str != '\0')
        str++;
    return str;
}

}

