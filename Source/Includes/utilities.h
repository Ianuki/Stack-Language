#ifndef UTILITIES_H
#define UTILITIES_H

#include "lexer.h"

static void util_mem_println(int length, const char* buffer) {
    printf("%.*s\n", length, buffer);
}

#endif