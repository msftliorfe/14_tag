// direct_line_generator.h

#ifndef DIRECT_LINE_GENERATOR_H
#define DIRECT_LINE_GENERATOR_H

#include <stddef.h>

// Function to generate a 12-bit unsigned binary string from an integer
// (Assuming this function is declared in "number_handler.h")
// char* intTo12BitUnsignedString(int num);

// Function to generate the ARE bits for a direct addressing line
char* direct_are(int num);

// Function to generate a complete direct addressing line
char* generate_direct_line(int num);

#endif // DIRECT_LINE_GENERATOR_H
