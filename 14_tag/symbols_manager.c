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

	// Initialize Symbols array
	manager->array = (Symbols*)malloc(5 * sizeof(Symbols)); // Initial size of 5
	if (manager->array == NULL) {
		perror("Failed to allocate memory for Symbols array");
		free(manager);
		exit(EXIT_FAILURE);
	}
	manager->used = 0;
	manager->size = 5;

	// Initialize ext array
	manager->ext = (char**)malloc(5 * sizeof(char*)); // Initial size of 5
	if (manager->ext == NULL) {
		perror("Failed to allocate memory for ext array");
		free(manager->array);
		free(manager);
		exit(EXIT_FAILURE);
	}
	manager->ext_used = 0;
	manager->ext_size = 5;

	// Initialize ent array
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

	// Initialize ref_symbols array
	manager->ref_symbols = (ReferenceSymbol*)malloc(5 * sizeof(ReferenceSymbol)); // Initial size of 5
	if (manager->ref_symbols == NULL) {
		perror("Failed to allocate memory for ref_symbols array");
		free(manager->ent);
		free(manager->ext);
		free(manager->array);
		free(manager);
		exit(EXIT_FAILURE);
	}
	manager->ref_used = 0;
	manager->ref_size = 5;

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
	printf("\n\n");
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
		if (isSymbolPattern(line[0])) {
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
	printf("\n\n");
	printf("manager->ext\n");
	size_t i;
	printf("| %-20s |\n", "Ext Name");
	printf("|----------------------|\n");

	for (i = 0; i < manager->ext_used; i++) {
		printf("| %-20s |\n", manager->ext[i]);
	}
}

void printEnt(const SymbolsManager* manager) {
	printf("\n\n");
	printf("manager->ent\n");
	size_t i;
	printf("| %-20s |\n", "Ent Name");
	printf("|----------------------|\n");

	for (i = 0; i < manager->ent_used; i++) {
		printf("| %-20s |\n", manager->ent[i]);
	}
}

bool isSymbolPattern(const char* word) {
	return word[(strlen(word) - 1)] == ':';
}

bool isDataPattern(const char* word) {
	return strcmp(word, ".data") == 0 || strcmp(word, ".string") == 0;
}

bool isReferencePattern(const char* word) {
	return strcmp(word, ".extern") == 0 || strcmp(word, ".entry") == 0;
}

void updateDataSymbolsLocation(const SymbolsManager* manager, int steps) {
	if (manager == NULL || manager->array == NULL) {
		return;
	}
	for (size_t i = 0; i < manager->used; ++i) {
		manager->array[i].symbol_location += (100 + (manager->array[i].is_data ? steps : 0));
	}
}

// Adding the function to add a reference symbol
void addReferenceSymbol(SymbolsManager* manager, const char* name, int location, bool type) {
	if (manager->ref_used == manager->ref_size) {
		manager->ref_size *= 2;
		ReferenceSymbol* new_ref_symbols = (ReferenceSymbol*)realloc(manager->ref_symbols, manager->ref_size * sizeof(ReferenceSymbol));
		if (new_ref_symbols == NULL) {
			perror("Failed to reallocate memory for ReferenceSymbol array");
			free(manager->ref_symbols);
			free(manager->ent);
			free(manager->ext);
			free(manager->array);
			free(manager);
			exit(EXIT_FAILURE);
		}
		manager->ref_symbols = new_ref_symbols;
	}
	manager->ref_symbols[manager->ref_used].name = strdup(name);
	if (manager->ref_symbols[manager->ref_used].name == NULL) {
		perror("Failed to duplicate name");
		free(manager->ref_symbols);
		free(manager->ent);
		free(manager->ext);
		free(manager->array);
		free(manager);
		exit(EXIT_FAILURE);
	}
	manager->ref_symbols[manager->ref_used].location = location;
	manager->ref_symbols[manager->ref_used].type = type;
	manager->ref_used++;
}

// Adding the function to print all reference symbols
void printReferenceSymbols(const SymbolsManager* manager) {
	printf("\n\n");
	printf("manager->ReferenceSymbols\n");
	printf("| %-20s | %-10s | %-5s |\n", "Name", "Location", "Type");
	printf("|----------------------|------------|-------|\n");

	for (size_t i = 0; i < manager->ref_used; i++) {
		printf("| %-20s | %-10d | %-5d |\n", manager->ref_symbols[i].name, manager->ref_symbols[i].location, manager->ref_symbols[i].type);
	}
}

bool isRefExtSymbolExists(const SymbolsManager* manager, const char* symbol_name) {
	if (manager == NULL || symbol_name == NULL) {
		return false; // Return 0 if the manager or symbol_name is NULL
	}

	for (size_t i = 0; i < manager->ext_used; i++) {
		if (strcmp(manager->ext[i], symbol_name) == 0) {
			return true; // Return true if the symbol_name exists in the ext array
		}
	}

	return false; // Return false if the symbol_name does not exist in the ext array
}
bool isRefEntSymbolExists(const SymbolsManager* manager, const char* symbol_name) {
	if (manager == NULL || symbol_name == NULL) {
		return false; // Return 0 if the manager or symbol_name is NULL
	}

	for (size_t i = 0; i < manager->ent_used; i++) {
		if (strcmp(manager->ent[i], symbol_name) == 0) {
			return true; // Return true if the symbol_name exists in the ext array
		}
	}

	return false; // Return false if the symbol_name does not exist in the ext array
}



