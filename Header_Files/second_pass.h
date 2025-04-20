/* second_pass.h */

#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "symbol_table.h"
#include "parser_old.h"

/**
 * @brief Executes the second pass of the assembler.
 *
 * This function resolves label addresses, processes .entry, and prepares final output.
 *
 * @param am_file The input assembly file (.am)
 * @param symbol_table The symbol table from first pass
 * @param symbol_count Number of symbols
 * @param code Machine code array (partially built)
 * @param code_count Number of code words
 * @param data Data segment
 * @param data_count Number of data words
 * @param IC Final IC value after first pass
 * @param DC Final DC value after first pass
 * @param error_flag Set to 1 if any error occurs
 * @return 1 on success, 0 on failure
 */
int second_pass(const char *am_file, Symbol *symbol_table, int symbol_count,
                unsigned short *code, int code_count,
                unsigned short *data, int data_count,
                int IC, int DC, int *error_flag);

#endif
