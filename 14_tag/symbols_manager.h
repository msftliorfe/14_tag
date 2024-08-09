#include <stdbool.h>

#ifndef SYMBOLS_HANDLER_H
#define SYMBOLS_HANDLER_H

typedef struct {
	char* symbol_name;
	int symbol_location;
	bool is_data;
} Symbols;


// New structure for reference symbols
typedef struct {
	char* name;
	int location;
	bool type; // true for ext, false for ent
} ReferenceSymbol;


struct SymbolsManager {
	Symbols* array;
	size_t used;
	size_t size;
	char** ext;
	size_t ext_used;
	size_t ext_size;
	char** ent;
	size_t ent_used;
	size_t ent_size;
	ReferenceSymbol* ref_symbols;
	size_t ref_used;
	size_t ref_size;
};

typedef struct SymbolsManager SymbolsManager;





// Function to add a reference symbol to the manager
void addReferenceSymbol(SymbolsManager* manager, const char* name, int location, bool type);

// Function to print all reference symbols
void printReferenceSymbols(const SymbolsManager* manager);


// Function to create a SymbolsManager instance
SymbolsManager* createSymbolsManager(void);

// Function to add a symbol to the manager
void addSymbol(SymbolsManager* manager, const char* symbol_name, int symbol_location, bool is_data);

// Function to print all symbols
void printSymbols(const SymbolsManager* manager);

// Function to get the location of a symbol by its name
int getSymbolLocation(const SymbolsManager* manager, const char* symbol_name);
bool isRefExtSymbolExists(const SymbolsManager* manager, const char* symbol_name);
bool isRefEntSymbolExists(const SymbolsManager* manager, const char* symbol_name);

// Function to destroy a SymbolsManager instance and free resources
void destroySymbolsManager(SymbolsManager* manager);

// Function to add a value to ext or ent
void addExtEnt(SymbolsManager* manager, const char* value, bool is_ext);

// Function to update the symbols table
void updateSymbolsTable(SymbolsManager* manager, char** line, int location);

// Function to check if an action exists
void printExt(const SymbolsManager* manager);
void printEnt(const SymbolsManager* manager);

bool isSymbolPattern(const char* action_name);


bool isDataPattern(const char* word);

bool isReferencePattern(const char* word);

void updateDataSymbolsLocation(const SymbolsManager* manager, int steps);

#endif // SYMBOLS_HANDLER_H
