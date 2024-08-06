#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "number_handler.h"
#include "labels_manager.h"


char* direct_are(int num) {// the num is a location of a label based on the LabelsManager
	if (num == 0) { // external labal
		return "001";
	}
	return "010";
}

char* generate_direct_line(int num) { // the num is a location of a label based on the LabelsManager
	char* binaryString = intTo12BitUnsignedString(num);
	char* staticString = are(num);

	if (!binaryString || !staticString) {
		if (binaryString) free(binaryString); // Free memory if only one allocation succeeded
		return NULL; // Return NULL if any string is NULL
	}

	// Allocate memory for the combined result
	size_t combinedLength = strlen(binaryString) + strlen(staticString) + 1;
	char* combinedString = (char*)malloc(combinedLength);
	if (!combinedString) {
		free(binaryString); // Free the binary string if memory allocation failed
		return NULL; // Memory allocation failed
	}

	// Combine the strings
	strcpy(combinedString, binaryString);
	strcat(combinedString, staticString);

	// Free the allocated memory for the binary string
	free(binaryString);

	return combinedString;
}