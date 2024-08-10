#ifndef OPERANDS_H
#define OPERANDS_H

#include <stdbool.h>

bool is_valid_regiter(const char* operand);

typedef enum AddressingType {
	Direct,
	Immediate,
	DirectRegister,
	IndirectRegister
} AddressingType;

AddressingType get_addressing_type(const char* operand);


#endif // OPERANDS_H
