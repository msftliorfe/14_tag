#include "macro_manager.h"
#include <stdlib.h>
#include <string.h>

void init_macro_manager(MacroManager* manager) {
    manager->macro_count = 0;
    manager->is_macro_context = false;
}

char** macro_manager_builder(MacroManager* manager, char** input, size_t input_count) {
    size_t i, j;

    if (input_count == 0) return NULL;

    if (manager->is_macro_context) {
        for (i = 0; i < manager->macro_count; ++i) {
            if (strcmp(manager->macro_names[i], manager->current_macro_name) == 0) {
                Macro* macro = &manager->macros[i];
                macro->commands = realloc(macro->commands, (macro->command_count + 1) * sizeof(char*));
                if (macro->commands == NULL) {
                    // Handle allocation failure
                    return NULL;
                }
                macro->commands[macro->command_count] = malloc(MAX_STRING_LENGTH * sizeof(char));
                if (macro->commands[macro->command_count] == NULL) {
                    // Handle allocation failure
                    return NULL;
                }
                strcpy(macro->commands[macro->command_count], input[0]);
                for (j = 1; j < input_count; ++j) {
                    strcat(macro->commands[macro->command_count], " ");
                    strcat(macro->commands[macro->command_count], input[j]);
                }
                ++macro->command_count;
                return NULL;
            }
        }
    }

    if (strcmp(input[0], "macr") == 0) {
        manager->is_macro_context = true;
        strcpy(manager->current_macro_name, input[1]);
        manager->macro_names[manager->macro_count] = malloc(MAX_STRING_LENGTH * sizeof(char));
        if (manager->macro_names[manager->macro_count] == NULL) {
            // Handle allocation failure
            return NULL;
        }
        strcpy(manager->macro_names[manager->macro_count], input[1]);
        manager->macros[manager->macro_count].commands = NULL;
        manager->macros[manager->macro_count].command_count = 0;
        ++manager->macro_count;
        return NULL;
    }
    else if (strcmp(input[0], "endmacr") == 0) {
        manager->is_macro_context = false;
        return NULL;
    }

    for (i = 0; i < manager->macro_count; ++i) {
        if (strcmp(manager->macro_names[i], input[0]) == 0) {
            char** result = malloc(manager->macros[i].command_count * sizeof(char*));
            if (result == NULL) {
                // Handle allocation failure
                return NULL;
            }
            for (j = 0; j < manager->macros[i].command_count; ++j) {
                result[j] = malloc((strlen(manager->macros[i].commands[j]) + 1) * sizeof(char));
                if (result[j] == NULL) {
                    // Handle allocation failure
                    return NULL;
                }
                strcpy(result[j], manager->macros[i].commands[j]);
            }
            return result;
        }
    }

    return input;
}

void free_macro_manager(MacroManager* manager) {
    size_t i, j;
    for (i = 0; i < manager->macro_count; ++i) {
        for (j = 0; j < manager->macros[i].command_count; ++j) {
            free(manager->macros[i].commands[j]);
        }
        free(manager->macros[i].commands);
        free(manager->macro_names[i]);
    }
}
