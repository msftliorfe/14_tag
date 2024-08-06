#ifndef MACRO_MANAGER_H
#define MACRO_MANAGER_H

#include <stdbool.h>
#include <stddef.h>

#define MAX_STRING_LENGTH 100
#define MAX_MACROS 100

typedef struct {
    char*** commands;  // Matrix of strings (commands)
    size_t row_count;  // Number of rows in the matrix
} Macro;

typedef struct {
    Macro macros[MAX_MACROS];
    char* macro_names[MAX_MACROS];
    size_t macro_count;
    bool is_macro_context;
    char current_macro_name[MAX_STRING_LENGTH];
} MacroManager;

void init_macro_manager(MacroManager* manager);
char** process_file_line(MacroManager* manager, char** input, size_t input_count);
void free_macro_manager(MacroManager* manager);
bool is_macro_name(MacroManager* manager, const char* name);

#endif // MACRO_MANAGER_H
