#include "macro_manager.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h> // For debugging

void init_macro_manager(MacroManager* manager) {
	manager->macro_count = 0;
	manager->is_macro_context = false;
}

char** process_file_line(MacroManager* manager, char** input, size_t input_count) {
	size_t i, j;

	if (input_count == 0) return NULL;

	if (strcmp(input[0], "endmacr") == 0) {
		manager->is_macro_context = false;
		return NULL;
	}

	if (manager->is_macro_context) {
		for (i = 0; i < manager->macro_count; ++i) {
			if (strcmp(manager->macro_names[i], manager->current_macro_name) == 0) {
				Macro* macro = &manager->macros[i];
				macro->commands = realloc(macro->commands, (macro->row_count + 1) * sizeof(char**));
				if (macro->commands == NULL) {
					// Handle allocation failure
					return NULL;
				}
				macro->commands[macro->row_count] = malloc((input_count + 1) * sizeof(char*));
				if (macro->commands[macro->row_count] == NULL) {
					// Handle allocation failure
					return NULL;
				}
				for (j = 0; j < input_count; ++j) {
					macro->commands[macro->row_count][j] = malloc((strlen(input[j]) + 1) * sizeof(char));
					if (macro->commands[macro->row_count][j] == NULL) {
						// Handle allocation failure
						return NULL;
					}
					strcpy(macro->commands[macro->row_count][j], input[j]);
				}
				macro->commands[macro->row_count][input_count] = NULL; // Null-terminate the row
				macro->row_count++;
				return NULL;
			}
		}
	}

	if (strcmp(input[0], "macr") == 0) {
		manager->is_macro_context = true;
		strcpy(manager->current_macro_name, input[1]);
		manager->macro_names[manager->macro_count] = malloc((strlen(input[1]) + 1) * sizeof(char));
		if (manager->macro_names[manager->macro_count] == NULL) {
			// Handle allocation failure
			return NULL;
		}
		strcpy(manager->macro_names[manager->macro_count], input[1]);
		manager->macros[manager->macro_count].commands = NULL;
		manager->macros[manager->macro_count].row_count = 0;
		manager->macro_count++;
		return NULL;
	}

	for (i = 0; i < manager->macro_count; ++i) {
		if (strcmp(manager->macro_names[i], input[0]) == 0) {
			Macro* macro = &manager->macros[i];
			char** result = malloc((macro->row_count + 1) * sizeof(char*));
			if (result == NULL) {
				// Handle allocation failure
				return NULL;
			}
			for (j = 0; j < macro->row_count; ++j) {
				size_t k, row_length = 0;
				for (k = 0; macro->commands[j][k] != NULL; ++k) {
					row_length += strlen(macro->commands[j][k]) + 1;
				}
				result[j] = malloc((row_length + 1) * sizeof(char));
				if (result[j] == NULL) {
					// Handle allocation failure
					return NULL;
				}
				result[j][0] = '\0';
				for (k = 0; macro->commands[j][k] != NULL; ++k) {
					strcat(result[j], macro->commands[j][k]);
					if (macro->commands[j][k + 1] != NULL) {
						strcat(result[j], " ");
					}
				}
			}
			result[macro->row_count] = NULL; // Null-terminate the result array
			return result;
		}
	}

	// Allocate new memory for processed_line
	char** processed_line = malloc((input_count + 1) * sizeof(char*));
	if (processed_line == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}
	for (i = 0; i < input_count; ++i) {
		processed_line[i] = malloc((strlen(input[i]) + 1) * sizeof(char));
		if (processed_line[i] == NULL) {
			fprintf(stderr, "Memory allocation failed\n");
			exit(EXIT_FAILURE);
		}
		strcpy(processed_line[i], input[i]);
	}
	processed_line[input_count] = NULL; // Null-terminate the processed line

	return processed_line;
}


void free_macro_manager(MacroManager* manager) {
	size_t i, j, k;
	for (i = 0; i < manager->macro_count; ++i) {
		for (j = 0; j < manager->macros[i].row_count; ++j) {
			for (k = 0; manager->macros[i].commands[j][k] != NULL; ++k) {
				free(manager->macros[i].commands[j][k]);
			}
			free(manager->macros[i].commands[j]);
		}
		free(manager->macros[i].commands);
		free(manager->macro_names[i]);
	}
}
