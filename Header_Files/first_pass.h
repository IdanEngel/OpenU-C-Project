#ifndef FIRST_PASS_H
#define FIRST_PASS_H

/* Builds the symbol table from the expanded assembly file (.am) */
void first_pass(const char *filename);
void handle_instruction(const Operation *op, const char *operand_line, int address);
void add_code_row(int address);

#endif
