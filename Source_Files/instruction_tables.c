/* This file contains all static instruction metadata to be used by the assembler */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "../Header_Files/instruction_tables.h"

#define MAX_OPCODE_NAME 10
#define MAX_OPERANDS 2


/* Table of all operations */
const Operation operations_table[] = {
    {"mov", 0, 0, 2},
    {"cmp", 1, 0, 2},
    {"add", 2, 1, 2},
    {"sub", 2, 2, 2},
    {"lea", 4, 0, 2},
    {"clr", 5, 1, 1},
    {"not", 5, 2, 1},
    {"inc", 5, 3, 1},
    {"dec", 5, 4, 1},
    {"jmp", 9, 1, 1},
    {"bne", 9, 2, 1},
    {"jsr", 9, 3, 1},
    {"red", 12, 0, 1},
    {"prn", 13, 0, 1},
    {"rts", 14, 0, 0},
    {"stop", 15, 0, 0}
};

const int operations_count = sizeof(operations_table) / sizeof(operations_table[0]);

/* Determine if a string is a register (r0 - r7) */
int is_register(const char* token) {
    return strlen(token) == 2 && token[0] == 'r' && token[1] >= '0' && token[1] <= '7';
}

/* Find operation by name */
const Operation* get_operation(const char* name) {
      int i;
      for (i = 0; i < operations_count; i++) {
        if (strcmp(operations_table[i].name, name) == 0) {
            return &operations_table[i];
        }
    }
    return NULL;
}

/* Identify addressing mode from operand */
AddressingMode get_addressing_mode(const char* operand) {
    if (operand[0] == '#') return IMMEDIATE;
    if (operand[0] == '&') return RELATIVE;
    if (is_register(operand)) return REGISTER;
    return DIRECT;
}

/* Get the expected number of operands (registers) for an opcode */
int get_expected_register_count(const char* opcode_name) {
    const Operation* op = get_operation(opcode_name);
    if (!op) return -1;
    return op->operand_count;
}

