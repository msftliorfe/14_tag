#ifndef ACTION_GENERATOR_H
#define ACTION_GENERATOR_H

#include <stdbool.h>
#include "symbols_manager.h"

#define ACTION_CODE_LENGTH 4
#define OPERAND_CODE_LENGTH 4
#define TOTAL_LENGTH (ACTION_CODE_LENGTH + 2 * OPERAND_CODE_LENGTH + 1) // Adjust based on your needs

// Function to generate operand code based on the provided operand string
char* generate_operand_code(const char* operand);

// Function to generate the first line of output based on the given parameters
char* generate_first_line(const char* action_name, const char* operand_target, const char* operand_source);

char* process_first_line(const char** line);

#endif // ACTION_GENERATOR_H
