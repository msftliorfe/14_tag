#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbols_manager.h"

SymbolsManager* createSymbolsManager() {
    SymbolsManager* manager = malloc(sizeof(SymbolsManager));
    if (manager == NULL) {
        perror("Failed to create LabelsManager");
        exit(EXIT_FAILURE);
    }
    manager->array = malloc(5 * sizeof(Symbols)); // Initial size of 5
    if (manager->array == NULL) {
        perror("Failed to allocate memory for Labels array");
        free(manager);
        exit(EXIT_FAILURE);
    }
    manager->used = 0;
    manager->size = 5;
    return manager;
}

void addSymbol(SymbolsManager* manager, const char* symbol_name, int symbol_location) {
    if (manager->used == manager->size) {
        manager->size *= 2;
        Symbols* new_array = realloc(manager->array, manager->size * sizeof(Symbols));
        if (new_array == NULL) {
            perror("Failed to reallocate memory for Labels array");
            free(manager->array);
            free(manager);
            exit(EXIT_FAILURE);
        }
        manager->array = new_array;
    }
    manager->array[manager->used].symbol_name = strdup(symbol_name); // Make a copy of the string
    if (manager->array[manager->used].symbol_name == NULL) {
        perror("Failed to duplicate label_name");
        free(manager->array);
        free(manager);
        exit(EXIT_FAILURE);
    }
    manager->array[manager->used].symbol_location = symbol_location;
    manager->used++;
}

void printSymbols(const SymbolsManager* manager) {
    for (size_t i = 0; i < manager->used; i++) {
        printf("Label: %s, Location: %d\n", manager->array[i].symbol_name, manager->array[i].symbol_location);
    }
}

int getSymbolLocation(const SymbolsManager* manager, const char* symbol_name) {
    for (size_t i = 0; i < manager->used; i++) {
        if (strcmp(manager->array[i].symbol_name, symbol_name) == 0) {
            return manager->array[i].symbol_location;
        }
    }
    return -1; // Indicate that the label was not found
}

void destroySymbolsManager(SymbolsManager* manager) {
    for (size_t i = 0; i < manager->used; i++) {
        free(manager->array[i].symbol_name); // Free the copied strings
    }
    free(manager->array);
    free(manager);
}
