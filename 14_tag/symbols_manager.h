#include <stdbool.h>
#ifndef Symbols_HANDLER_H
#define Symbols_HANDLER_H

typedef struct {
	char* symbol_name;
	int symbol_location;
	bool is_relocatable;
	bool is_external;
	bool is_entry;
	bool is_data;
} Symbols;

struct SymbolsManager {
	Symbols* array;
	size_t used;
	size_t size;
};

typedef struct SymbolsManager SymbolsManager;

// Function to create a LabelsManager instance
SymbolsManager* createSymbolsManager(void);

// Function to add a label to the manager
void addSymbol(SymbolsManager* manager, const char* symbol_name, int symbol_location);

// Function to print all labels
void printSymbols(const SymbolsManager* manager);

// Function to get the location of a label by its name
int getSymbolLocation(const SymbolsManager* manager, const char* symbol_name);

// Function to destroy a LabelsManager instance and free resources
void destroySymbolsManager(SymbolsManager* manager);

#endif // Symbols_HANDLER_H
