/* codegen.c */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../Header_Files/codegen.h"
#include "../Header_Files/utils.h"

#define OPCODE_MOV 0
#define OPCODE_CMP 1
#define OPCODE_ADD 2
#define OPCODE_SUB 3
#define OPCODE_NOT 4
#define OPCODE_CLR 5
#define OPCODE_LEA 6
#define OPCODE_INC 7
#define OPCODE_DEC 8
#define OPCODE_JMP 9
#define OPCODE_BNE 10
#define OPCODE_RED 11
#define OPCODE_PRN 12
#define OPCODE_JSR 13
#define OPCODE_RTS 14
#define OPCODE_STOP 15

static int get_opcode(const char *mnemonic) {
    const char *table[] = {
        "mov", "cmp", "add", "sub", "not", "clr", "lea", "inc",
        "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"
    };
    int i;
    for (i = 0; i < 16; i++) {
        if (strcmp(mnemonic, table[i]) == 0)
            return i;
    }
    return -1;
}

static int is_register(const char *arg) {
    return (arg != NULL && strlen(arg) == 3 && arg[0] == '@' && arg[1] == 'r' &&
            arg[2] >= '0' && arg[2] <= '7');
}

static int is_immediate(const char *arg) {
    return (arg != NULL && arg[0] == '#');
}

static int encode_register(const char *reg) {
    return reg[2] - '0';  /* '@r3' → 3 */
}

int encode_instruction_stub(Line line, unsigned short *code, int *IC) {
    int opcode = get_opcode(line.opcode);
    int word = 0;

    if (opcode < 0) return 0;

    /* Base word */
    word |= (opcode << 6);  /* Bits 6-9 = opcode */

    /* Addressing modes for args */
    if (line.arg_count == 2) {
        /* Source operand */
        if (is_immediate(line.args[0]))
            word |= (1 << 10);  /* Immediate addressing = 01 */
        else if (is_register(line.args[0]))
            word |= (3 << 10);  /* Register = 11 */
        else
            word |= (2 << 10);  /* Direct label = 10 */

        /* Dest operand */
        if (is_register(line.args[1]))
            word |= (3 << 2);
        else if (is_immediate(line.args[1]))
            word |= (1 << 2);
        else
            word |= (2 << 2);
    }

    code[(*IC)++] = word;

    /* Add extra words for operands */
    if (line.arg_count == 2) {
        if (is_immediate(line.args[0])) {
            code[(*IC)++] = (unsigned short)atoi(line.args[0] + 1);
        } else if (!is_register(line.args[0])) {
            code[(*IC)++] = 9999; /* Placeholder for label */
        }

        if (is_immediate(line.args[1])) {
            code[(*IC)++] = (unsigned short)atoi(line.args[1] + 1);
        } else if (!is_register(line.args[1])) {
            code[(*IC)++] = 9999; /* Placeholder for label */
        }
    } else if (line.arg_count == 1) {
        if (is_immediate(line.args[0])) {
            code[(*IC)++] = (unsigned short)atoi(line.args[0] + 1);
        } else if (!is_register(line.args[0])) {
            code[(*IC)++] = 9999; /* Placeholder for label */
        }
    }

    return 1;
}
