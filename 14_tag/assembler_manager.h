#ifndef ASSEMBLER_MANAGER_H
#define ASSEMBLER_MANAGER_H
#include "assembler_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include "file_manager.h"
#include "symbols_manager.h"
// Define the AssemblerManager struct
typedef struct {
	int IC;
	int DC;
	int SC;
} AssemblerManager;

// Function prototypes
AssemblerManager* createAssemblerManager();
void destroyAssemblerManager(AssemblerManager* manager);
void generat_symbols_table(SymbolsManager* symbolsManager, FileManager* fileManager, AssemblerManager* assemblerManager);

#endif // ASSEMBLER_MANAGER_H
