#ifndef PARSER_H
#define PARSER_H

#include "../Header_Files/instruction_tables.h"

void int_to_binary(int value, int width, char *output);
void add_binary_instruction(int opcode, AddressingMode src_mode, AddressingMode dest_mode,
                            int src_reg, int dest_reg, int funct, int address);
void add_symbol_operand(const char *symbol_name, int address);
void encode_string_directive(const char *str, int base_address);
void encode_data_directive(const int *values, int count, int base_address);

/* Connect to first_pass */
void handle_string_directive(const char *label, const char *value, int *dc);
void handle_data_directive(const char *label, const int *values, int count, int *dc);


#endif /* PARSER_H */
