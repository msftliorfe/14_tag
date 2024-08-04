#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "actions.h"

// Define the actions array
Action actions[] = {
    {"mov","0", "0,1,2,3", "1,2,3"},
    {"cmp","1", "0,1,2,3", "0,1,2,3"},
    {"add","2", "0,1,2,3", "1,2,3"},
    {"sub","3", "0,1,2,3", "1,2,3"},
    {"lea","4", "1", "1,2,3"},
    {"clr","5", "no source operand", "1,2,3"},
    {"not","6", "no source operand", "1,2,3"},
    {"inc","7", "no source operand", "1,2,3"},
    {"dec","8", "no source operand", "1,2,3"},
    {"jmp","9", "no source operand", "1,2,3"},
    {"bne","10", "no source operand", "1,2"},
    {"red","11", "no source operand", "1,2"},
    {"prn","12", "no source operand", "1,2,3"},
    {"jsr","13", "no source operand", "0,1,2,3"},
    {"rts","14", "no source operand", "1,2"},
    {"stop","15", "no source operand", "no destination operand"}
};

// Calculate the number of actions
int num_actions = sizeof(actions) / sizeof(actions[0]);

// Define the function to print the actions
void print_actions(void) {
    int i; // Declare loop control variable outside the loop for ANSI C
    for (i = 0; i < num_actions; i++) {
        printf("Action: %s\n", actions[i].action_name);
        printf("Source Operands: %s\n", actions[i].source_operands);
        printf("Destination Operands: %s\n", actions[i].destination_operands);
        printf("\n");
    }
}

// Function to get source operands by action name
const char* get_source_operands(const char* action_name) {
    int i; // Declare loop control variable outside the loop for ANSI C
    for (i = 0; i < num_actions; i++) {
        if (strcmp(actions[i].action_name, action_name) == 0) {
            return actions[i].source_operands;
        }
    }
    return NULL; // Return NULL if action name not found
}

// Function to get destination operands by action name
const char* get_destination_operands(const char* action_name) {
    int i; // Declare loop control variable outside the loop for ANSI C
    for (i = 0; i < num_actions; i++) {
        if (strcmp(actions[i].action_name, action_name) == 0) {
            return actions[i].destination_operands;
        }
    }
    return NULL; // Return NULL if action name not found
}

// Function to get action code by action name
const char* get_action_code(const char* action_name) {
    int i; // Declare loop control variable outside the loop for ANSI C
    for (i = 0; i < num_actions; i++) {
        if (strcmp(actions[i].action_name, action_name) == 0) {
            return actions[i].action_code;
        }
    }
    return NULL; // Return NULL if action name not found
}
