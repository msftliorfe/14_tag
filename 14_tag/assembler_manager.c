#include "assembler_manager.h"
#include "symbols_manager.h"
#include "file_manager.h"

// Function to create and initialize an AssemblerManager
AssemblerManager* createAssemblerManager() {
	AssemblerManager* manager = (AssemblerManager*)malloc(sizeof(AssemblerManager));
	if (manager == NULL) {
		perror("Failed to create AssemblerManager");
		exit(EXIT_FAILURE);
	}
	manager->IC = 0;
	manager->DC = 0;
	manager->SC = 0;
	return manager;
}

void generat_symbols_table(SymbolsManager* symbolsManager, FileManager* fileManager, AssemblerManager* assemblerManager) {
	for (size_t i = 0; i < fileManager->row_count; ++i) {
		updateSymbolsTable(symbolsManager, fileManager->post_macro[i], assemblerManager->SC);
		assemblerManager->SC += 1;
	}
}

// Function to destroy an AssemblerManager
void destroyAssemblerManager(AssemblerManager* manager) {
	free(manager);
}
