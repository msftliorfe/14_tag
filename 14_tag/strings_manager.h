#ifndef STRINGS_H
#define STRINGS_H

#include <stdbool.h>

// Function prototype
bool is_null_or_empty(const char* str);

// Function to split a string by spaces and return an array of strings
char** split_string(const char* str);

// Function to free the allocated memory for the array of strings
void free_split_string(char** split_str);
char* letter_to_15bit_ascii(char letter);

#endif // STRINGS_H
