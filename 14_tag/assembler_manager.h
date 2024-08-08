#ifndef ASSEMBLER_MANAGER_H
#define ASSEMBLER_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include "file_manager.h"
#include "symbols_manager.h"

// Define the Item structure
typedef struct {
    int location;
    char value[16]; // 15 bits + 1 for null terminator
} Item;

// Define the AssemblerManager struct
typedef struct {
    int IC;
    int DC;
    Item* dataItems;
    size_t dataItemCount;
    Item* actionItems;
    size_t actionItemCount;
} AssemblerManager;

// Function prototypes
AssemblerManager* createAssemblerManager();
void destroyAssemblerManager(AssemblerManager* manager);
void first_scan(FileManager* fileManager, AssemblerManager* assemblerManager, SymbolsManager* symbolsManager);
void processActionLine(char** line, AssemblerManager* assemblerManager);
void processDataLine(char** line, AssemblerManager* assemblerManager);
void addDataItem(AssemblerManager* manager, int location, const char* value);
void addActionItem(AssemblerManager* manager, int location, const char* value);
void printDataItems(const AssemblerManager* manager);
void printActionItems(const AssemblerManager* manager);

#endif // ASSEMBLER_MANAGER_H
