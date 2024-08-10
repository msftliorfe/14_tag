#include <stdio.h>
#include "actions.h"
#include "file_manager.h"
#include "symbols_manager.h"
#include "number_handler.h"
#include "macro_manager.h"
#include "assembler_manager.h"
int main(int argc, char** argv) {
	// errors:
	// wrong number of operds for action
	// wrong addersing type
	//more then one symbol name
	// not exisiintg operand (in source or dest)
	// not exisintg regiser number (only 0-7 allowed)

	FileManager fileManager;
	initialize_file_manager(&fileManager);

	MacroManager macroManager;
	init_macro_manager(&macroManager);

	const char* file_path = "c_input_2_file.txt";

	input_process(&fileManager, &macroManager, file_path);

	print_post_macro(&fileManager);
	AssemblerManager* assemblerManager = createAssemblerManager();

	SymbolsManager* symbolsManager = createSymbolsManager();
	first_scan(&fileManager, assemblerManager, symbolsManager);
	updateLocationDataSymbols(symbolsManager, assemblerManager);
	updateDataItemsLocation(assemblerManager);


	//printSymbols(symbolsManager);
	//printEnt(symbolsManager);
	//printExt(symbolsManager);
	//printActionItems(assemblerManager);
	//printDataItems(assemblerManager);
	second_scan(&fileManager, assemblerManager, symbolsManager);
	/*printf("\n\n");
	printf("======================================");
	printf("\n");
	printf("after second scan");
	printf("\n");
	printActionItems(assemblerManager);
	printDataItems(assemblerManager);
	printSymbols(symbolsManager);
	printReferenceSymbols(symbolsManager);*/
	printObjToFile(assemblerManager);
	printReferenceSymbolsToFile(symbolsManager);
	free_file_manager(&fileManager);

	return 0;
}