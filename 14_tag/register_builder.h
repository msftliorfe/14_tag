#pragma once
#ifndef REGISTER_HANDLER_H
#define REGISTER_HANDLER_H

#include <stdbool.h>  // For using the 'bool' type

/**
 * Function to return the ARE string.
 * @return The ARE string.
 */
char* register_are();

/**
 * Function to generate a single register line.
 * @param register_number Number of the register (0-7).
 * @param is_source Boolean indicating if the register is a source.
 * @return The generated line as a string, or NULL on error.
 */
char* generate_single_register_line(int register_number, bool is_source);

/**
 * Function to generate a combined register line.
 * @param a_register_number Number of the first register (0-7).
 * @param b_register_number Number of the second register (0-7).
 * @return The generated line as a string, or NULL on error.
 */
char* generate_combined_register_line(int a_register_number, int b_register_number);

#endif // REGISTER_HANDLER_H
