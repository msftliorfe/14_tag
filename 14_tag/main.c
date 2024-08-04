#include <stdio.h>
#include "actions.h"
#include "row_handler.h"
#include "labels_handler.h"
#include "number_handler.h"

int main(int argc, char** argv) {
	for (int i = 0; i < argc; ++i)
	{
		printf("argv[%d]: %s\n", i, argv[i]);
	}
	// Print the actions
	//print_actions();


	char str[] = "  This  is   an example   string  ";
	char** split_str = split_string(str);
	printf("The words in the string are:\n");
	for (int i = 0; split_str[i] != NULL; i++) {
	    printf("%s\n", split_str[i]);
	}
	// Free the allocated memory
	free_split_string(split_str);
	// 
	// 
	// 
	// 
	int num = 5; // Example number
	char* binaryString = intTo12Bit2ComplementString(num);

	//if (binaryString) {
	//    printf("12-bit 2's complement: %s\n", binaryString);
	//    free(binaryString); // Don't forget to free the allocated memory
	//}
	//else {
	//    printf("Number out of range or memory allocation failed\n");
	//}



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

	return 0;
}
