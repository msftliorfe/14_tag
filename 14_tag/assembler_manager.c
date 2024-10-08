#include "assembler_manager.h"
#include "symbols_manager.h"
#include "file_manager.h"
#include "actions.h"
#include "data_manager.h"
#include "first_line_builder.h"
#include "strings_manager.h"
#include "operands.h"
#include <stdbool.h>
#include "immediate_builder.h"
#include "register_builder.h"
#include "number_handler.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "direct_builder.h"

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
	bool reg_dest_was_handled = false;
	char* first_line = process_first_line(line);
	addActionItem(assemblerManager, line[0], assemblerManager->IC, first_line);
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
					reg_dest_was_handled = true;
					int dest_reg_num = addressing_type_dest == DirectRegister ? line[2][1] - '0' : line[2][2] - '0';
					char* line_to_add = generate_combined_register_line(source_reg_num, dest_reg_num); // pass the reg number without * sign
					addActionItem(assemblerManager, "", assemblerManager->IC, line_to_add);

				}
				else {//DirectRegister or IndirectRegister in source and there is a dest but not DirectRegister or IndirectRegister in dest
					char* line_to_add = generate_single_register_line(source_reg_num, true); // pass the reg number without * sign
					addActionItem(assemblerManager, "", assemblerManager->IC, line_to_add);
				}
			}
			else { // DirectRegister or IndirectRegister in source but no dest operands at all
				char* line_to_add = generate_single_register_line(source_reg_num, true); // pass the reg number without * sign
				addActionItem(assemblerManager, "", assemblerManager->IC, line_to_add);
			}
		}
		else { // not DirectRegister or IndirectRegister in source only Immediate or Direct
			switch (addressing_type_source)
			{
			case Immediate: {
				int number = atoi(line[1] + 1);
				char* line_to_add = generate_immediate_line(number);
				addActionItem(assemblerManager, "", assemblerManager->IC, line_to_add);
				break;
			}
			case Direct: {
				addActionItem(assemblerManager, "LABEL", assemblerManager->IC, line[1]);
				break;
			}

			}
		}
	}
	if (has_dest_operands) { // handle dest operands, but skip DirectRegister and IndirectRegister if already handled
		int location_of_current_operand = has_source_operands ? 2 : 1;
		AddressingType	addressing_type_dest = get_addressing_type(line[location_of_current_operand]);
		switch (addressing_type_dest)
		{
		case Immediate: {
			int number = atoi(line[location_of_current_operand] + 1);
			char* line_to_add = generate_immediate_line(number);
			addActionItem(assemblerManager, "", assemblerManager->IC, line_to_add);
			break;
		}
		case Direct: {
			addActionItem(assemblerManager, "LABEL", assemblerManager->IC, line[location_of_current_operand]);
			break;
		}
		default: {
			if (!reg_dest_was_handled) { // combined row was handled before
				int dest_reg_num = addressing_type_dest == DirectRegister ? line[location_of_current_operand][1] - '0' : line[location_of_current_operand][2] - '0';
				char* line_to_add = generate_single_register_line(dest_reg_num, false); // pass the reg number without * sign
				addActionItem(assemblerManager, "", assemblerManager->IC, line_to_add);
			}
		}
			   break;
		}
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
	manager->dataItems[manager->dataItemCount].octal = bitStringToOctal(value);
	strncpy(manager->dataItems[manager->dataItemCount].value, value, 15);
	manager->dataItems[manager->dataItemCount].value[15] = '\0';
	manager->dataItemCount++;
}

void addActionItem(AssemblerManager* manager, char* metadata, int location, const char* value) {
	manager->actionItems = (Item*)realloc(manager->actionItems, (manager->actionItemCount + 1) * sizeof(Item));
	if (manager->actionItems == NULL) {
		perror("Failed to add action item");
		exit(EXIT_FAILURE);
	}
	manager->actionItems[manager->actionItemCount].location = 100 + location;
	manager->actionItems[manager->actionItemCount].octal = bitStringToOctal(value);
	strncpy(manager->actionItems[manager->actionItemCount].value, value, 15);
	manager->actionItems[manager->actionItemCount].value[15] = '\0';
	manager->actionItems[manager->actionItemCount].metadata = metadata;
	manager->actionItemCount++;
	manager->IC++;

}

void printItems(const Item* items, size_t itemCount, bool includeMetadata) {
	if (includeMetadata) {
		printf("| Location | Value           | Metadata    | Octal |\n");
		printf("|----------|-----------------|-------------|-------|\n");
		for (size_t i = 0; i < itemCount; ++i) {
			printf("| %8d | %-15s | %-11s | %5s |\n", items[i].location, items[i].value, items[i].metadata ? items[i].metadata : "", items[i].octal);
		}
	}
	else {
		printf("| Location | Value           | Octal |\n");
		printf("|----------|-----------------|-------|\n");
		for (size_t i = 0; i < itemCount; ++i) {
			printf("| %8d | %-15s | %5s |\n", items[i].location, items[i].value, items[i].octal);
		}
	}
}



void printDataItems(const AssemblerManager* manager) {
	printf("\n\n");

	printf("DataItems\n");
	printItems(manager->dataItems, manager->dataItemCount, false);
}

void printActionItems(const AssemblerManager* manager) {
	printf("\n\n");

	printf("ActionItems\n");
	printItems(manager->actionItems, manager->actionItemCount, true);
}

void updateLocationDataSymbols(const SymbolsManager* symbolsManager, const AssemblerManager* manager) {
	updateDataSymbolsLocation(symbolsManager, manager->IC);
}

void updateDataItemsLocation(const AssemblerManager* manager) {
	for (size_t i = 0; i < manager->dataItemCount; ++i) {
		manager->dataItems[i].location += 100 + manager->IC;
	}
}

void second_scan(FileManager* fileManager, AssemblerManager* assemblerManager, SymbolsManager* symbolsManager) {


	for (size_t i = 0; i < assemblerManager->actionItemCount; ++i) {
		Item* actionItem = &assemblerManager->actionItems[i];

		if (strcmp(actionItem->metadata, "LABEL") == 0) {
			actionItem->metadata = strdup(actionItem->value);  // Update metadata to be the value
			bool found_in_ext = false;
			if (isRefExtSymbolExists(symbolsManager, actionItem->value)) {// this is an ext label
				char* location_str = int_to_15bit_twos_complement(1);
				addReferenceSymbol(symbolsManager, actionItem->value, actionItem->location, true); // add new item to ref_symbols
				// Copy the new string into the value array
				strncpy(actionItem->value, location_str, sizeof(actionItem->value) - 1);
				actionItem->value[sizeof(actionItem->value) - 1] = '\0';  // Ensure null-termination
				actionItem->octal = bitStringToOctal(location_str);

				free(location_str);
			}
			else { // this is ent symbol or just symbol - find its location in symbols table
				int symbol_location = getSymbolLocation(symbolsManager, actionItem->value);
				char* location_str = generate_direct_line(symbol_location);
				// Copy the new string into the value array
				strncpy(actionItem->value, location_str, sizeof(actionItem->value) - 1);
				actionItem->octal = bitStringToOctal(location_str);

				free(location_str);

			}
		}
	}
	for (size_t i = 0; i < symbolsManager->ent_used; ++i) {// handle entry symbols
		char* entlItem = symbolsManager->ent[i];
		int symbol_location = getSymbolLocation(symbolsManager, entlItem);
		char* location_str = generate_direct_line(symbol_location);
		addReferenceSymbol(symbolsManager, entlItem, symbol_location, false); // add new item to ref_symbols
		free(location_str);
	}
}

void printObjToFile(const AssemblerManager* assemblerManager) {
	FILE* file = fopen("ps.obj", "w");
	if (file == NULL) {
		perror("Failed to open file ps.obj");
		exit(EXIT_FAILURE);
	}

	// Print the first line: IC tab_space DC
	fprintf(file, "%d\t%d\n", assemblerManager->IC, assemblerManager->DC);

	// Print actionItems
	for (size_t i = 0; i < assemblerManager->actionItemCount; ++i) {
		fprintf(file, "%d\t%s\n", assemblerManager->actionItems[i].location, assemblerManager->actionItems[i].octal);
	}

	// Print dataItems
	for (size_t i = 0; i < assemblerManager->dataItemCount; ++i) {
		fprintf(file, "%d\t%s\n", assemblerManager->dataItems[i].location, assemblerManager->dataItems[i].octal);
	}

	fclose(file);
}

void printReferenceSymbolsToFile(const SymbolsManager* manager) {
	FILE* ent_file = NULL;
	FILE* ext_file = NULL;
	bool ent_has_values = false;
	bool ext_has_values = false;
	size_t i;

	for (i = 0; i < manager->ref_used; ++i) {
		ReferenceSymbol* ref_symbol = &manager->ref_symbols[i];
		if (ref_symbol->type) {
			if (!ext_has_values) {
				ext_file = fopen("ps.ext", "w");
				if (ext_file == NULL) {
					perror("Failed to open file ps.ext");
					exit(EXIT_FAILURE);
				}
				ext_has_values = 1;
			}
			fprintf(ext_file, "%s\t%d\n", ref_symbol->name, ref_symbol->location);
		}
		else {
			if (!ent_has_values) {
				ent_file = fopen("ps.ent", "w");
				if (ent_file == NULL) {
					perror("Failed to open file ps.ent");
					exit(EXIT_FAILURE);
				}
				ent_has_values = 1;
			}
			fprintf(ent_file, "%s\t%d\n", ref_symbol->name, ref_symbol->location);
		}
	}

	if (ext_file != NULL) {
		fclose(ext_file);
	}
	if (ent_file != NULL) {
		fclose(ent_file);
	}
}
