#include <stdbool.h>
#ifndef ACTIONS_H
#define ACTIONS_H

// Define a struct to hold the action details
typedef struct {
	const char* action_name;
	const int action_code;
	const char* source_operands;
	const char* destination_operands;
} Action;

// Function to print the actions
void print_actions(void);

// Function to get source operands by action name
const char* get_source_operands(const char* action_name);

// Function to get destination operands by action name
const char* get_destination_operands(const char* action_name);

// Function to get action code by action name
const char* get_action_code(const char* action_name);
bool action_exists(const char* action_name);

#endif // ACTIONS_H
