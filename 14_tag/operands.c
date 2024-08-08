#include "operands.h"
#include <string.h>
#include <stdbool.h> // Ensure this is included for bool

// Array of valid operands
static const char* valid_regiters[] = { "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7" };
static const char* valid_regiters_2[] = { "*r0", "*r1", "*r2", "*r3", "*r4", "*r5", "*r6", "*r7" };
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

bool is_valid_regiter_2(const char* operand) {
	for (size_t i = 0; i < num_registers; ++i) {
		if (strcmp(valid_regiters_2[i], operand) == 0) {
			return true;
		}
	}
	return false;
}
//Operand_Type get_operand_type(const char* operand) {
//	if (is_valid_regiter(operand)) {
//		return Register;
//	}
//	else if (strcmp(operand[0], "#") == 0) {
//
//		return Number;
//	}
//	return Symbol;
//}

AddressingType get_addressing_type(const char* operand) {
	if (is_valid_regiter(operand)) {
		return DirectRegister;
	}
	else if (is_valid_regiter_2(operand)) {
		return IndirectRegister;
	}
	else if (operand[0] == '#') {
		return Immediate;
	}
	return Direct;
}

