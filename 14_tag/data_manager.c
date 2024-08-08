#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_manager.h"

/* Assume these existing functions are defined elsewhere */
extern char* int_to_15bit_twos_complement(int number);
extern char* letter_to_15bit_ascii(char letter);

char** handle_numbers(char** number_strings) {
	char** result;
	int count = 0;
	int i;

	/* Calculate the number of strings in the array */
	while (number_strings[count] != NULL) {
		count++;
	}

	result = (char**)malloc(count * sizeof(char*));
	if (result == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < count; i++) {
		int number = atoi(number_strings[i]);
		result[i] = int_to_15bit_twos_complement(number);
	}

	return result;
}

char** handle_strings(const char* input_string) {
	int length, i;
	char** result;

	length = strlen(input_string);
	result = (char**)malloc((length + 1) * sizeof(char*));
	if (result == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < length; i++) {
		result[i] = letter_to_15bit_ascii(input_string[i]);
	}
	result[length] = NULL;  /* NULL-terminate the array */

	return result;
}


char** generateDataLine(const char* input_array) {
	if (input_array[0] == ".data") {
		return handle_numbers(input_array);
	}
	else {
		return handle_strings(input_array);

	}
}