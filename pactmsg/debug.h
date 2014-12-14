#ifndef PACT_DEBUG_H
#define PACT_DEBUG_H

#include "config.h"
#include <stdio.h>

#define pact_debug_print(fmt, ...) do { if (PACT_DEBUG_PRINT) fprintf(stderr, fmt, __VA_ARGS__); } while (0)

#define pact_debug_write(str) do { pact_debug_print("%s\n", str); } while (0)

#endif