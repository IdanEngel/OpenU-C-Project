#ifndef INSTRUCTION_TABLES_H
#define INSTRUCTION_TABLES_H

#define MAX_OPCODE_NAME 10

typedef enum {
    IMMEDIATE = 0,   /* #value*/
    DIRECT = 1,      /* label*/
    RELATIVE = 2,    /* &label*/
    REGISTER = 3     /* r0-r7*/
} AddressingMode;

typedef struct {
    char name[MAX_OPCODE_NAME];
    int opcode;
    int funct;
    int operand_count; /* 0, 1, or 2*/
} Operation;

extern const Operation operations_table[];
extern const int operations_count;

int is_register(const char* token);
const Operation* get_operation(const char* name);
AddressingMode get_addressing_mode(const char* operand);
int get_expected_register_count(const char* opcode_name);
void int_to_binary(int value, int width, char *output);


#endif