#ifndef ROW_HANDLER_H
#define ROW_HANDLER_H

// Function to split a string by spaces and return an array of strings
char** split_string(const char* str);

// Function to free the allocated memory for the array of strings
void free_split_string(char** split_str);

#endif // ROW_HANDLER_H
