#include "assembler_manager.h"
#include "symbols_manager.h"
#include "file_manager.h"
#include "actions.h"
#include "data_manager.h"
#include "first_line_builder.h"
#include "strings_manager.h"
#include "operands.h"
#include <stdbool.h> // Include for the bool type

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
	char* action_name = clone_string(line[0]);
	char* source_operands = get_source_operands(action_name);
	char* destination_operands = get_destination_operands(action_name);
	bool has_source_operands = strcmp(source_operands, "-1") != 0;
	bool has_dest_operands = strcmp(destination_operands, "-1") != 0;
	if (has_source_operands) { // there are source_operands for this action
		AddressingType	addressing_type_source = get_addressing_type(line[1]);
		if (addressing_type_source == DirectRegister || addressing_type_source == IndirectRegister) {
			int source_reg_num = addressing_type_source == DirectRegister ? line[1][1] - '0' : line[1][2] - '0';
			if (has_dest_operands) {
				AddressingType	addressing_type_dest = get_addressing_type(line[2]);
				if (addressing_type_dest == DirectRegister || addressing_type_dest == IndirectRegister) {
					int dest_reg_num = addressing_type_dest == DirectRegister ? line[2][1] - '0' : line[2][2] - '0';
					char* line_to_add = generate_combined_register_line(source_reg_num, dest_reg_num); // pass the reg number without * sign
					addActionItem(assemblerManager, assemblerManager->IC, line_to_add);

				}
				else {//DirectRegister or IndirectRegister in source but not DirectRegister or IndirectRegister in dest
					char* line_to_add = generate_single_register_line(source_reg_num, source_reg_num); // pass the reg number without * sign
					addActionItem(assemblerManager, assemblerManager->IC, line_to_add);
					switch (addressing_type_dest)
					{
					case Immediate: {
						int number = atoi(line[1] + 1);
						char* line_to_add = generate_immediate_line(number);
						addActionItem(assemblerManager, assemblerManager->IC, line_to_add);
						break;
					}
					case Direct: {
						addActionItem(assemblerManager, assemblerManager->IC, "LABEL_PLACE_HOLDER");
						break;
					}

					}
				}
			}
			else { // DirectRegister or IndirectRegister in source but doesnt have dest operands
				char* line_to_add = generate_single_register_line(source_reg_num, true); // pass the reg number without * sign
				addActionItem(assemblerManager, assemblerManager->IC, line_to_add);
			}
		}
		else { // not DirectRegister or IndirectRegister in source
			switch (addressing_type_source)
			{
			case Immediate: {
				int number = atoi(line[1] + 1);
				char* line_to_add = generate_immediate_line(number);
				addActionItem(assemblerManager, assemblerManager->IC, line_to_add);
				break;
			}
			case Direct: {
				addActionItem(assemblerManager, assemblerManager->IC, "LABEL_PLACE_HOLDER");
				break;
			}

			}
		}
	}
	else { // no source operands

	}
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
	manager->IC++;

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
