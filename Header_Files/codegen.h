/* codegen.h */

#ifndef CODEGEN_H
#define CODEGEN_H
#define NULL ((void *)0)
#define MAX_LINE_LENGTH 31
#include "symbol_table.h"

typedef struct {
    char *label;          /* e.g., "LOOP" from "LOOP: mov ..." */
    char *opcode;         /* e.g., "mov" or ".data" */
    char *args[2];        /* Up to 2 arguments */
    int is_data;          /* 1 if .data or .string */
    int is_extern;        /* 1 if .extern */
    int is_entry;         /* 1 if .entry */
    int is_command;       /* 1 if opcode is real command (mov, cmp, ...) */
    int arg_count;        /* 0, 1, or 2 */
    int error;            /* 1 if parsing error occurred */
    char error_msg[100];  /* Explanation of the error */
    char operands[MAX_LINE_LENGTH];
} Line;

/**
 * @brief Encodes a command into machine code (partial, label-resolved later).
 *
 * @param line Parsed line containing opcode and args
 * @param code The code array to store result
 * @param IC Pointer to instruction counter (will be incremented)
 * @return 1 if successful, 0 on error
 */
int encode_instruction_binary(Line line, unsigned int *code, int *IC);

#endif
