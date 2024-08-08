#include "assembler_manager.h"
#include "symbols_manager.h"
#include "file_manager.h"
#include "actions.h"
#include "data_manager.h"
#include "first_line_builder.h"
// Function to create and initialize an AssemblerManager
AssemblerManager* createAssemblerManager() {
	AssemblerManager* manager = (AssemblerManager*)malloc(sizeof(AssemblerManager));
	if (manager == NULL) {
		perror("Failed to create AssemblerManager");
		exit(EXIT_FAILURE);
	}
	manager->IC = 0;
	manager->DC = 0;
	manager->dataItems = NULL;
	manager->dataItemCount = 0;
	manager->actionItems = NULL;
	manager->actionItemCount = 0;
	return manager;
}

// Function to destroy an AssemblerManager
void destroyAssemblerManager(AssemblerManager* manager) {
	free(manager->dataItems);
	free(manager->actionItems);
	free(manager);
}

void first_scan(FileManager* fileManager, AssemblerManager* assemblerManager, SymbolsManager* symbolsManager) {
	for (size_t i = 0; i < fileManager->row_count; ++i) {
		char** line = fileManager->post_macro[i];

		if (action_exists(line[0]) || isSymbolPattern(line[0]) || isDataPattern(line[0]) || isReferencePattern(line[0])) {
			if (isReferencePattern(line[0])) {
				updateSymbolsTable(symbolsManager, line, -1);
			}
			else if (isSymbolPattern(line[0])) {
				if (isDataPattern(line[1])) {
					updateSymbolsTable(symbolsManager, line, assemblerManager->DC);
					processDataLine(line + 1, assemblerManager);
				}
				else if (action_exists(line[1])) {
					updateSymbolsTable(symbolsManager, line, assemblerManager->IC);
					processActionLine(line + 1, assemblerManager);
				}
			}
			else if (action_exists(line[0])) {
				processActionLine(line, assemblerManager);
			}
			else if (isDataPattern(line[0])) {
				processDataLine(line, assemblerManager);
			}
		}
	}
}

void processActionLine(char** line, AssemblerManager* assemblerManager) {
	// Example processing, needs actual implementation
	//addActionItem(assemblerManager, assemblerManager->IC, *line);
	char* first_line = process_first_line(line);
	addActionItem(assemblerManager, assemblerManager->IC, first_line);
	assemblerManager->IC++;
}

void processDataLine(char** line, AssemblerManager* assemblerManager) {
	char** data_lines = generateDataLine(line);
	int count = 0;

	while (data_lines[count] != NULL) {
		addDataItem(assemblerManager, assemblerManager->DC, data_lines[count]);
		count++;
		assemblerManager->DC++;
	}
}

void addDataItem(AssemblerManager* manager, int location, const char* value) {
	manager->dataItems = (Item*)realloc(manager->dataItems, (manager->dataItemCount + 1) * sizeof(Item));
	if (manager->dataItems == NULL) {
		perror("Failed to add data item");
		exit(EXIT_FAILURE);
	}
	manager->dataItems[manager->dataItemCount].location = location;
	strncpy(manager->dataItems[manager->dataItemCount].value, value, 15);
	manager->dataItems[manager->dataItemCount].value[15] = '\0';
	manager->dataItemCount++;
}

void addActionItem(AssemblerManager* manager, int location, const char* value) {
	manager->actionItems = (Item*)realloc(manager->actionItems, (manager->actionItemCount + 1) * sizeof(Item));
	if (manager->actionItems == NULL) {
		perror("Failed to add action item");
		exit(EXIT_FAILURE);
	}
	manager->actionItems[manager->actionItemCount].location = location;
	strncpy(manager->actionItems[manager->actionItemCount].value, value, 15);
	manager->actionItems[manager->actionItemCount].value[15] = '\0';
	manager->actionItemCount++;
}

void printItems(const Item* items, size_t itemCount) {
	printf("| Location | Value           |\n");
	printf("|----------|-----------------|\n");
	for (size_t i = 0; i < itemCount; ++i) {
		printf("| %8d | %-15s |\n", items[i].location, items[i].value);
	}
}

void printDataItems(const AssemblerManager* manager) {
	printf("DataItems\n");
	printItems(manager->dataItems, manager->dataItemCount);
}

void printActionItems(const AssemblerManager* manager) {
	printf("ActionItems\n");
	printItems(manager->actionItems, manager->actionItemCount);
}
