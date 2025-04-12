/* codegen.h */

#ifndef CODEGEN_H
#define CODEGEN_H

#include "parser.h"
#include "symbol_table.h"

/**
 * @brief Encodes a command into machine code (partial, label-resolved later).
 *
 * @param line Parsed line containing opcode and args
 * @param code The code array to store result
 * @param IC Pointer to instruction counter (will be incremented)
 * @return 1 if successful, 0 on error
 */
int encode_instruction_stub(Line line, unsigned short *code, int *IC);

#endif
