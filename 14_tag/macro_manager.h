#ifndef MACRO_MANAGER_H
#define MACRO_MANAGER_H

#include <stdbool.h>
#include <stddef.h>

#define MAX_STRING_LENGTH 100

typedef struct {
    char** commands;
    size_t command_count;
} Macro;

typedef struct {
    Macro macros[100];
    char* macro_names[100];
    size_t macro_count;
    bool is_macro_context;
    char current_macro_name[MAX_STRING_LENGTH];
} MacroManager;

void init_macro_manager(MacroManager* manager);
char** macro_manager_builder(MacroManager* manager, char** input, size_t input_count);
void free_macro_manager(MacroManager* manager);

#endif // MACRO_MANAGER_H
