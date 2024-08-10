#pragma once
#ifndef REGISTER_HANDLER_H
#define REGISTER_HANDLER_H

#include <stdbool.h>  

char* register_are();
char* generate_single_register_line(int register_number, bool is_source);

char* generate_combined_register_line(int a_register_number, int b_register_number);

#endif // REGISTER_HANDLER_H
