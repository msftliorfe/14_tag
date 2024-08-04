#include "operands.h"
#include <string.h>
#include <stdbool.h> // Ensure this is included for bool

// Array of valid operands
static const char* valid_operands[] = { "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7" };
static const size_t num_operands = sizeof(valid_operands) / sizeof(valid_operands[0]);

/**
 * Check if a given operand is valid.
 * @param operand The operand to check.
 * @return True if the operand is valid, false otherwise.
 */
bool is_valid_operand(const char* operand) {
    for (size_t i = 0; i < num_operands; ++i) {
        if (strcmp(valid_operands[i], operand) == 0) {
            return true;
        }
    }
    return false;
}
