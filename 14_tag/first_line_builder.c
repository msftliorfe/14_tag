#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "symbols_manager.h"
#include "actions.h"
#include "strings_manager.h"
#include "operands.h"
#include "number_handler.h"

#define ACTION_CODE_LENGTH 4
#define OPERAND_CODE_LENGTH 4
#define TOTAL_LENGTH (ACTION_CODE_LENGTH + 2 * OPERAND_CODE_LENGTH + 1) // Adjust based on your needs

// Helper function to generate operand code and return it as a new string
char* generate_operand_code(const char* operand) {
    char* code = malloc(OPERAND_CODE_LENGTH + 1); // Allocate memory for the code + null terminator
    if (code == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    if (!is_null_or_empty(operand)) {
        char first_letter = operand[0];
        switch (first_letter) {
        case '#':
            strcpy(code, "0001");
            break;
        case '*':
            strcpy(code, "0100");
            break;
        default:
            if (is_valid_operand(operand)) {
                strcpy(code, "1000");
            }
            else {
                strcpy(code, "0010");
            }
            break;
        }
    }
    else {
        strcpy(code, "0000");
    }

    return code;
}

  char* generate_first_line(SymbolsManager* manager, const char* action_name, const char* operand_source, const char* operand_target) {
    // Allocate memory for result string
    char* res = malloc(TOTAL_LENGTH + 1); // +1 for the null terminator
    if (res == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Initialize result string
    res[0] = '\0';

    // Generate action code (11-14)
    char* action_code = get_action_code(action_name);
    int action_code_number = action_code ? (action_code[0] - '0') : 0; // Ensure action_code is not NULL
    char* action_code_string = intTo4BitString(action_code_number);
    strcat(res, action_code_string);
    free(action_code_string); // Free the allocated memory

    // Generate operand source (7-10)
    char* operand_source_code = generate_operand_code(operand_source);
    strcat(res, operand_source_code);
    free(operand_source_code); // Free the allocated memory

    // Generate operand target (3-6)
    char* operand_target_code = generate_operand_code(operand_target);
    strcat(res, operand_target_code);
    free(operand_target_code); // Free the allocated memory

    strcat(res, "100");
    return res;
}
