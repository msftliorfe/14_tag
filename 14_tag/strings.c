#include "strings.h"  // Make sure this header file contains the function prototype
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * Checks if the given string is NULL or empty.
 * @param str Pointer to the string to be checked.
 * @return true if the string is NULL or empty, false otherwise.
 */
bool is_null_or_empty(const char* str) {
    return (str == NULL || *str == '\0');
}
