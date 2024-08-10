#include <stdio.h>
#include "actions.h"
#include "file_manager.h"
#include "symbols_manager.h"
#include "number_handler.h"
#include "macro_manager.h"
#include "assembler_manager.h"
int main(int argc, char** argv) {
	FileManager fileManager;
	initialize_file_manager(&fileManager);

	MacroManager macroManager;
	init_macro_manager(&macroManager);

	const char* file_path = "c_input_2_file.txt";

	input_process(&fileManager, &macroManager, file_path);

	// Print the post_macro matrix
	print_post_macro(&fileManager);
	AssemblerManager* assemblerManager = createAssemblerManager();

	SymbolsManager* symbolsManager = createSymbolsManager();
	//generat_symbols_table(symbolsManager, &fileManager, assemblerManager);
	first_scan(&fileManager, assemblerManager, symbolsManager);
	updateLocationDataSymbols(symbolsManager, assemblerManager);
	updateDataItemsLocation(assemblerManager);
	//process_post_macro(&fileManager, assemblerManager, symbolsManager);


	printSymbols(symbolsManager);
	printEnt(symbolsManager);
	printExt(symbolsManager);
	printActionItems(assemblerManager);
	printDataItems(assemblerManager);
	second_scan(&fileManager, assemblerManager, symbolsManager);
	printf("\n\n");
	printf("======================================");
	printf("\n");
	printf("after second scan");
	printf("\n");
	printActionItems(assemblerManager);
	printDataItems(assemblerManager);
	printSymbols(symbolsManager);
	printReferenceSymbols(symbolsManager);
	printObjToFile(assemblerManager);
	printReferenceSymbolsToFile(symbolsManager);
	free_file_manager(&fileManager);

	return 0;
}