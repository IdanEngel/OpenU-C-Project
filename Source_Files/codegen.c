/* codegen.c */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../Header_Files/codegen.h"
#include "../Header_Files/instruction_tables.h"
#include "../Header_Files/first_pass.h"
#include "../Header_Files/utils.h"

#define OPCODE_MOV 0
#define OPCODE_CMP 1
#define OPCODE_ADD 2
#define OPCODE_SUB 3
#define OPCODE_NOT 4
#define OPCODE_CLR 5
#define OPCODE_LEA 4
#define OPCODE_INC 7
#define OPCODE_DEC 8
#define OPCODE_JMP 9
#define OPCODE_BNE 10
#define OPCODE_RED 11
#define OPCODE_PRN 12
#define OPCODE_JSR 13
#define OPCODE_RTS 14
#define OPCODE_STOP 15

/* Table of all operations */
const Operation operations[] = {
    {"mov", 0, 0},
    {"cmp", 1, 0},
    {"add", 2, 1},
    {"sub", 2, 2},
    {"lea", 4, 0},
    {"clr", 5, 1},
    {"not", 5, 2},
    {"inc", 5, 3},
    {"dec", 5, 4},
    {"jmp", 9, 1},
    {"bne", 9, 2},
    {"jsr", 9, 3},
    {"red", 12, 0},
    {"prn", 13, 0},
    {"rts", 14, 0},
    {"stop", 15, 0}
};

/*
 * Find and return the opcode value for a given mnemonic (operation name).
 * Returns -1 if the mnemonic is not found.
 */
static int get_opcode(const char *mnemonic) {
    int i;
    for (i = 0; i < 16; i++) {
        if (strcmp(mnemonic, operations[i].name) == 0)
            return operations[i].opcode;
    }
    return -1;
}
/*
 * Find and return the funct value (secondary function code) for a given operation name.
 * Returns -1 if the operation is not found.
 */
int get_funct(const char *opcode_name) {
    int i;
    for (i = 0; i < 16; i++) {
        if (strcmp(opcode_name, operations[i].name) == 0)
            return operations[i].funct;
    }
    return -1;
}
/*
 * Convert an integer value into a binary string representation of a given width.
 * The output is stored as a null-terminated string in the 'output' buffer.
 */
void int_to_binary(const int value, const int width, char *output) {
    int i;
    for (i = width - 1; i >= 0; i--) {
        output[width - 1 - i] = ((value >> i) & 1) ? '1' : '0';
    }
    output[width] = '\0';
}
/*
 * Encode a parsed instruction (given as a Line struct) into machine code words.
 * Fills the 'code' array starting from the position pointed by 'IC'.
 * Returns 1 on success, or 0 if an invalid opcode was encountered.
 */
int encode_instruction_binary(Line line, unsigned int *code, int *IC) {
    const int opcode = get_opcode(line.opcode);
    const int funct = get_funct(line.opcode);
    unsigned int word = 0;
    int src_mode = 0, dest_mode = 0;
    int src_reg = 0, dest_reg = 0;

    if (opcode < 0) return 0;

    if (line.arg_count == 2) {
        src_mode = get_addressing_mode(line.args[0]);
        dest_mode = get_addressing_mode(line.args[1]);

        if (is_register(line.args[0])) {
            if (line.args[0][0] == '@')
                src_reg = line.args[0][2] - '0';
            else
                src_reg = line.args[0][1] - '0';
        }
        if (is_register(line.args[1])) {
            if (line.args[1][0] == '@')
                dest_reg = line.args[1][2] - '0';
            else
                dest_reg = line.args[1][1] - '0';
        }
    } else if (line.arg_count == 1) {
        dest_mode = get_addressing_mode(line.args[0]);
        if (is_register(line.args[0])) {
            if (line.args[0][0] == '@')
                dest_reg = line.args[0][2] - '0';
            else
                dest_reg = line.args[0][1] - '0';
        }
    }

    word |= (opcode & 0x3F) << 18;
    word |= (src_mode & 0x03) << 16;
    word |= (src_reg & 0x07) << 13;
    word |= (dest_mode & 0x03) << 11;
    word |= (dest_reg & 0x07) << 8;
    word |= (funct & 0x1F) << 3;
    word |= 4; /* ARE = 100 (absolute) */

    code[(*IC)++] = word;

    if (line.arg_count >= 1) {
        int i;
        int val;
        unsigned int bin;
        for (i = 0; i < line.arg_count; i++) {
            if (line.args[i][0] == '#') {
                val = atoi(line.args[i] + 1);
                bin = ((unsigned int)(val & 0x1FFFFF) << 3);  /* Keep 21 bits */
                bin |= 4;  /* ARE = 100 */
                code[(*IC)++] = bin;
            } else if (is_register(line.args[i])) {
                continue;
            } else {
                code[(*IC)++] = 0x000002;
            }
        }
    }

    return 1;
}
