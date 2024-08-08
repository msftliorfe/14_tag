#include <stdlib.h>

#include "number_handler.h"

char* intTo12BitUnsignedString(unsigned int num) {
	if (num > 4095) {
		return NULL;
	}
	char* binaryString = (char*)malloc(BITS + 1);
	if (!binaryString) {
		return NULL;
	}
	binaryString[BITS] = '\0';
	unsigned int mask = 1 << (BITS - 1);
	for (int i = 0; i < BITS; i++) {
		binaryString[i] = (num & mask) ? '1' : '0';
		mask >>= 1;
	}
	return binaryString;
}

char* intTo12Bit2ComplementString(int num) {
	if (num < -2048 || num > 2047) {
		return NULL;
	}
	char* binaryString = (char*)malloc(BITS + 1);
	if (!binaryString) {
		return NULL;
	}
	binaryString[BITS] = '\0';
	unsigned int mask = 1 << (BITS - 1);
	for (int i = 0; i < BITS; i++) {
		binaryString[i] = (num & mask) ? '1' : '0';
		mask >>= 1;
	}
	return binaryString;
}

char* intToThreeBitBinary(int num) {
	if (num < 0 || num > 7) {
		return NULL;
	}
	char* binaryString = (char*)malloc(4);
	if (!binaryString) {
		return NULL;
	}
	binaryString[0] = (num & 4) ? '1' : '0';
	binaryString[1] = (num & 2) ? '1' : '0';
	binaryString[2] = (num & 1) ? '1' : '0';
	binaryString[3] = '\0';
	return binaryString;
}

char* intTo4BitString(int number) {
	if (number < 0 || number > 15) {
		return NULL;
	}
	char* bitString = (char*)malloc(5);
	if (!bitString) {
		return NULL;
	}
	for (int i = 3; i >= 0; i--) {
		bitString[3 - i] = (number & (1 << i)) ? '1' : '0';
	}
	bitString[4] = '\0';
	return bitString;
}


char* int_to_15bit_twos_complement(int number) {
	char* result = (char*)malloc(16 * sizeof(char));
	int i;
	unsigned short twos_complement;

	if (result == NULL) {
		perror("Failed to allocate memory");
		exit(EXIT_FAILURE);
	}

	if (number < -16384 || number > 16383) {
		free(result);
		return NULL;
	}

	if (number < 0) {
		twos_complement = (unsigned short)((~(-number) + 1) & 0x7FFF);
	}
	else {
		twos_complement = (unsigned short)(number & 0x7FFF);
	}

	for (i = 14; i >= 0; i--) {
		result[14 - i] = (twos_complement & (1 << i)) ? '1' : '0';
	}
	result[15] = '\0';

	return result;
}

int calc_array_length(char** array) {
	int arraySize = 0;
	while (array[arraySize] != NULL) {
		arraySize++;
	}
	return array;
}

