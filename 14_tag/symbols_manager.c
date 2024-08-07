#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbols_manager.h"
#include "actions.h"
#include "strings_manager.h"

SymbolsManager* createSymbolsManager() {
	SymbolsManager* manager = (SymbolsManager*)malloc(sizeof(SymbolsManager));
	if (manager == NULL) {
		perror("Failed to create SymbolsManager");
		exit(EXIT_FAILURE);
	}
	manager->array = (Symbols*)malloc(5 * sizeof(Symbols)); // Initial size of 5
	if (manager->array == NULL) {
		perror("Failed to allocate memory for Symbols array");
		free(manager);
		exit(EXIT_FAILURE);
	}
	manager->used = 0;
	manager->size = 5;

	manager->ext = (char**)malloc(5 * sizeof(char*)); // Initial size of 5
	if (manager->ext == NULL) {
		perror("Failed to allocate memory for ext array");
		free(manager->array);
		free(manager);
		exit(EXIT_FAILURE);
	}
	manager->ext_used = 0;
	manager->ext_size = 5;

	manager->ent = (char**)malloc(5 * sizeof(char*)); // Initial size of 5
	if (manager->ent == NULL) {
		perror("Failed to allocate memory for ent array");
		free(manager->ext);
		free(manager->array);
		free(manager);
		exit(EXIT_FAILURE);
	}
	manager->ent_used = 0;
	manager->ent_size = 5;

	return manager;
}

void addSymbol(SymbolsManager* manager, const char* symbol_name, int symbol_location, bool is_data) {
	if (manager->used == manager->size) {
		manager->size *= 2;
		Symbols* new_array = (Symbols*)realloc(manager->array, manager->size * sizeof(Symbols));
		if (new_array == NULL) {
			perror("Failed to reallocate memory for Symbols array");
			free(manager->array);
			free(manager);
			exit(EXIT_FAILURE);
		}
		manager->array = new_array;
	}
	manager->array[manager->used].symbol_name = strdup(symbol_name); // Make a copy of the string
	if (manager->array[manager->used].symbol_name == NULL) {
		perror("Failed to duplicate symbol_name");
		free(manager->array);
		free(manager);
		exit(EXIT_FAILURE);
	}
	manager->array[manager->used].symbol_location = symbol_location;
	manager->array[manager->used].is_data = is_data;
	manager->used++;
}

void printSymbols(const SymbolsManager* manager) {
	printf("manager->Symbols\n");
	printf("| %-20s | %-10s | %-5s |\n", "Symbol Name", "Location", "Data");
	printf("|----------------------|------------|-------|\n");

	for (size_t i = 0; i < manager->used; i++) {
		printf("| %-20s | %-10d | %-5d |\n", manager->array[i].symbol_name, manager->array[i].symbol_location, manager->array[i].is_data);
	}
}

int getSymbolLocation(const SymbolsManager* manager, const char* symbol_name) {
	for (size_t i = 0; i < manager->used; i++) {
		if (strcmp(manager->array[i].symbol_name, symbol_name) == 0) {
			return manager->array[i].symbol_location;
		}
	}
	return -1; // Indicate that the symbol was not found
}

void destroySymbolsManager(SymbolsManager* manager) {
	for (size_t i = 0; i < manager->used; i++) {
		free(manager->array[i].symbol_name); // Free the copied strings
	}
	for (size_t i = 0; i < manager->ext_used; i++) {
		free(manager->ext[i]); // Free the ext strings
	}
	for (size_t i = 0; i < manager->ent_used; i++) {
		free(manager->ent[i]); // Free the ent strings
	}
	free(manager->array);
	free(manager->ext);
	free(manager->ent);
	free(manager);
}

void addExtEnt(SymbolsManager* manager, const char* value, bool is_ext) {
	if (is_ext) {
		if (manager->ext_used == manager->ext_size) {
			manager->ext_size *= 2;
			char** new_ext = (char**)realloc(manager->ext, manager->ext_size * sizeof(char*));
			if (new_ext == NULL) {
				perror("Failed to reallocate memory for ext array");
				free(manager->ext);
				free(manager->array);
				free(manager->ent);
				free(manager);
				exit(EXIT_FAILURE);
			}
			manager->ext = new_ext;
		}
		manager->ext[manager->ext_used] = strdup(value); // Make a copy of the string
		if (manager->ext[manager->ext_used] == NULL) {
			perror("Failed to duplicate value");
			free(manager->ext);
			free(manager->array);
			free(manager->ent);
			free(manager);
			exit(EXIT_FAILURE);
		}
		manager->ext_used++;
	}
	else {
		if (manager->ent_used == manager->ent_size) {
			manager->ent_size *= 2;
			char** new_ent = (char**)realloc(manager->ent, manager->ent_size * sizeof(char*));
			if (new_ent == NULL) {
				perror("Failed to reallocate memory for ent array");
				free(manager->ent);
				free(manager->array);
				free(manager->ext);
				free(manager);
				exit(EXIT_FAILURE);
			}
			manager->ent = new_ent;
		}
		manager->ent[manager->ent_used] = strdup(value); // Make a copy of the string
		if (manager->ent[manager->ent_used] == NULL) {
			perror("Failed to duplicate value");
			free(manager->ent);
			free(manager->array);
			free(manager->ext);
			free(manager);
			exit(EXIT_FAILURE);
		}
		manager->ent_used++;
	}
}


void updateSymbolsTable(SymbolsManager* symbolsManager, char** line, int location) {
	if (strcmp(line[0], ".extern") == 0) {
		addExtEnt(symbolsManager, line[1], true);
	}
	else if (strcmp(line[0], ".entry") == 0) {
		addExtEnt(symbolsManager, line[1], false);
	}
	else {
		size_t len = strlen(line[0]);
		if (line[0][len - 1] == ':') {
			char* symbol_name = strtrimlast(line[0]);
			if (symbol_name == NULL) {
				perror("Failed to duplicate symbol_name");
				exit(EXIT_FAILURE);
			}

			if (action_exists(line[1])) {
				addSymbol(symbolsManager, symbol_name, location, false);
			}
			else if (strcmp(line[1], ".string") == 0 || strcmp(line[1], ".data") == 0) {
				addSymbol(symbolsManager, symbol_name, location, true);
			}

			free(symbol_name);
		}
	}
}

void printExt(const SymbolsManager* manager) {
	printf("manager->ext\n");
	size_t i;
	printf("| %-20s |\n", "Ext Name");
	printf("|----------------------|\n");

	for (i = 0; i < manager->ext_used; i++) {
		printf("| %-20s |\n", manager->ext[i]);
	}
}

void printEnt(const SymbolsManager* manager) {
	printf("manager->ent\n");
	size_t i;
	printf("| %-20s |\n", "Ent Name");
	printf("|----------------------|\n");

	for (i = 0; i < manager->ent_used; i++) {
		printf("| %-20s |\n", manager->ent[i]);
	}
}


