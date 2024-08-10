#ifndef ASSEMBLER_MANAGER_H
#define ASSEMBLER_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include "file_manager.h"
#include "symbols_manager.h"

typedef struct {
	int location;
	char* metadata;
	char value[16];
	char* octal;
} Item;


typedef struct {
	int IC;
	int DC;
	Item* dataItems;
	size_t dataItemCount;
	Item* actionItems;
	size_t actionItemCount;
} AssemblerManager;

AssemblerManager* createAssemblerManager();
void destroyAssemblerManager(AssemblerManager* manager);
void first_scan(FileManager* fileManager, AssemblerManager* assemblerManager, SymbolsManager* symbolsManager);
void processActionLine(char** line, AssemblerManager* assemblerManager);
void processDataLine(char** line, AssemblerManager* assemblerManager);
void addDataItem(AssemblerManager* manager, int location, const char* value);
void addActionItem(AssemblerManager* manager, char* metadata, int location, const char* value);
void printDataItems(const AssemblerManager* manager);
void printActionItems(const AssemblerManager* manager);
void updateLocationDataSymbols(const SymbolsManager* symbolsManager, const AssemblerManager* manager);
void updateDataItemsLocation(const AssemblerManager* manager);
void second_scan(FileManager* fileManager, AssemblerManager* assemblerManager, SymbolsManager* symbolsManager);
void printObjToFile(const AssemblerManager* assemblerManager);
void printReferenceSymbolsToFile(const SymbolsManager* manager);
#endif // ASSEMBLER_MANAGER_H
