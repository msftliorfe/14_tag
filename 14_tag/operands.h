#ifndef OPERANDS_H
#define OPERANDS_H

#include <stdbool.h>

// Function declaration
bool is_valid_regiter(const char* operand);

typedef  enum Operand_Type {
	Register,
	Symbol,
	Number
} Operand_Type;

typedef enum AddressingType {
	Direct,
	Immediate,
	DirectRegister,
	IndirectRegister
} AddressingType;

Operand_Type get_operand_type(const char* operand);
AddressingType get_addressing_type(const char* operand);


#endif // OPERANDS_H
