#include <stdio.h>
#include "actions.h"
#include "file_manager.h"
#include "symbols_manager.h"
#include "number_handler.h"
#include "macro_manager.h"
#include "assembler_manager.h"
int main(int argc, char** argv) {

	//actions tester
	//----------------
	/*for (int i = 0; i < argc; ++i)
	{
		printf("argv[%d]: %s\n", i, argv[i]);
	}*/
	// Print the actions
	//print_actions();


	//strings tester
	// //---------------
	//char str[] = "  This  is   an example   string  ";
	//char** split_str = split_string(str);
	//printf("The words in the string are:\n");
	//for (int i = 0; split_str[i] != NULL; i++) {
	//    printf("%s\n", split_str[i]);
	//}
	//// Free the allocated memory
	//free_split_string(split_str);
	// 
	// 
	// 
	// 
	//int num = 5; // Example number
	//char* binaryString = intTo12Bit2ComplementString(num);
	//if (binaryString) {
	//    printf("12-bit 2's complement: %s\n", binaryString);
	//    free(binaryString); // Don't forget to free the allocated memory
	//}
	//else {
	//    printf("Number out of range or memory allocation failed\n");
	//}


	// labels tester
	// //------------------
	//LabelsManager* manager = createLabelsManager();

	//addLabel(manager, "start", 100);
	//addLabel(manager, "loop", 200);
	//addLabel(manager, "end", 300);

	//printLabels(manager);

	//const char* searchLabel = "loop";
	//const char* invalid_searchLabel = "loop!";
	//int location = getLabelLocation(manager, invalid_searchLabel);
	//printf("The location of label '%s' is %d\n", invalid_searchLabel, location);
	//destroyLabelsManager(manager); // Clean up

	// pre-compile-tester
	//--------------------------
	//MacroManager manager;
	//init_macro_manager(&manager);

	//char* input1[] = { "macr", "example_macro" };
	//process_file_line(&manager, input1, 2);

	//char* input2[] = { "example_command1", "arg1", "arg2" };
	//char* input5[] = { "example_command2", "arg3", "arg4" };
	//process_file_line(&manager, input2, 3);
	//process_file_line(&manager, input5, 3);

	//char* input3[] = { "endmacr" };
	//process_file_line(&manager, input3, 1);

	//char* input4[] = { "example_macro" };

	//char* input11[] = { "macr", "example_macro1" };
	//process_file_line(&manager, input11, 2);

	//char* input21[] = { "example_command11", "arg11", "arg21" };
	//char* input51[] = { "example_command21", "arg31", "arg41" };
	//process_file_line(&manager, input21, 3);
	//process_file_line(&manager, input51, 3);

	//char* input31[] = { "endmacr" };
	//process_file_line(&manager, input31, 1);

	//char* input41[] = { "example_macro1" };

	//char** result = process_file_line(&manager, input4, 1);
	//char** result1 = process_file_line(&manager, input41, 1);

	//if (result) {
	//	printf("example_macro:\n");
	//	for (size_t i = 0; result[i] != NULL; ++i) {
	//		printf("%s\n", result[i]);
	//		free(result[i]);
	//	}
	//	free(result);
	//}

	//if (result1) {
	//	printf("example_macro1:\n");
	//	for (size_t i = 0; result1[i] != NULL; ++i) {
	//		printf("%s\n", result1[i]);
	//		free(result1[i]);
	//	}
	//	free(result1);
	//}

	//free_macro_manager(&manager);


//FileManager test
//-------------
	FileManager fileManager;
	initialize_file_manager(&fileManager);

	MacroManager macroManager;
	init_macro_manager(&macroManager);

	const char* file_path = "C:\\Users\\liorgishry\\OneDrive - Microsoft\\Documents\\c_input_2_file.txt";

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


	free_file_manager(&fileManager);

	return 0;
}