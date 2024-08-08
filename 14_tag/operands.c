#include "operands.h"
#include <string.h>
#include <stdbool.h> // Ensure this is included for bool

// Array of valid operands
static const char* valid_regiters[] = { "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7" };
static const size_t num_registers = sizeof(valid_regiters) / sizeof(valid_regiters[0]);

/**
 * Check if a given operand is valid.
 * @param operand The operand to check.
 * @return True if the operand is valid, false otherwise.
 */
bool is_valid_regiter(const char* operand) {
	for (size_t i = 0; i < num_registers; ++i) {
		if (strcmp(valid_regiters[i], operand) == 0) {
			return true;
		}
	}
	return false;
}
