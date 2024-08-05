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

char** split_string(const char* str) {
    // Allocate an initial size for the result array
    size_t initial_size = 10;
    char** result = (char**)malloc(initial_size * sizeof(char*));
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    const char* start = str;
    size_t index = 0;
    while (*start) {
        while (*start == ' ' && *start != '\0') start++; // Skip leading spaces
        const char* end = start;
        while (*end != ' ' && *end != '\0') end++; // Find the end of the word

        if (end > start) {
            size_t len = (size_t)(end - start);
            result[index] = (char*)malloc((len + 1) * sizeof(char));
            if (result[index] == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }
            strncpy(result[index], start, len);
            result[index][len] = '\0';
            index++;

            // Reallocate memory if the result array is full
            if (index >= initial_size) {
                initial_size *= 2;
                char** temp = (char**)realloc(result, initial_size * sizeof(char*));
                if (temp == NULL) {
                    fprintf(stderr, "Memory reallocation failed\n");
                    exit(EXIT_FAILURE);
                }
                result = temp;
            }
        }
        start = end;
    }

    // Add a NULL pointer at the end of the array to indicate the end
    result[index] = NULL;

    return result;
}

// Function to free the allocated memory for the array of strings
void free_split_string(char** split_str) {
    if (split_str != NULL) {
        for (size_t i = 0; split_str[i] != NULL; i++) {
            free(split_str[i]);
        }
        free(split_str);
    }
}
