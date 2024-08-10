#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_manager.h"
#include "strings_manager.h";
#include "number_handler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** handle_numbers(char** number_strings) {
	char** result;
	int count = 0;
	int i;

	/* Calculate the number of strings in the array */
	while (number_strings[count] != NULL) {
		count++;
	}

	/* Allocate memory for the result array, including space for the null terminator */
	result = (char**)malloc((count + 1) * sizeof(char*));
	if (result == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < count; i++) {
		int number = atoi(number_strings[i]);
		result[i] = int_to_15bit_twos_complement(number);
	}

	/* Null-terminate the result array */
	result[count] = NULL;

	return result;
}


char** handle_strings(const char* input_string) {
	char* trimmed = remove_first_last(input_string);
	int length, i;
	char** result;
	const int extra_length = 1;  // Additional slot for the 15 "0"s string

	length = strlen(trimmed);
	result = (char**)malloc((length + extra_length) * sizeof(char*));
	if (result == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < length; i++) {
		result[i] = letter_to_15bit_ascii(trimmed[i]);
	}

	// Add the "000000000000000" string to the end of the result
	char* zero_string = (char*)malloc(16 * sizeof(char));  // 15 "0"s + NULL terminator
	if (zero_string == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}
	memset(zero_string, '0', 15);  // Fill with "0"s
	zero_string[15] = '\0';        // Null-terminate the string
	result[length] = zero_string;

	result[length + 1] = NULL;  // NULL-terminate the array

	free(trimmed);

	return result;
}



char** generateDataLine(const char** input_array) {
	if (strcmp(input_array[0], ".data") == 0) {
		return handle_numbers(input_array + 1);
	}
	else {
		return handle_strings(input_array[1]);

	}
}