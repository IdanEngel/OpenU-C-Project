/* Placeholder comment */
#include "../Header_Files/instruction_tables.h"
#include "../Header_Files/utils.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100
#define MAX_LABEL_LENGTH 31
#define MAX_SYMBOLS 100
#define MAX_CODE_ROWS 500

typedef enum { CODE, DATA, EXTERNAL, ENTRY } SymbolType;

typedef struct {
    char name[MAX_LABEL_LENGTH];
    int address;
    SymbolType type;
    int is_entry;
    int is_external;
} Symbol;

typedef struct {
    int address;
    char binary[25];
} CodeRow;

Symbol symbol_table[MAX_SYMBOLS];
int symbol_count = 0;
CodeRow code_table[MAX_CODE_ROWS];
int code_count = 0;
int IC = 100;
int DC = 0;
int ICF = 0;
int DCF = 0;

int is_comment_or_empty(const char *line) {
    return line[0] == ';' || line[0] == '\0';
}

void report_first_pass_error(int line_number, const char *message) {
    printf("[Line %d] Error: %s\n", line_number, message);
}

int is_valid_label(const char *label) {
    int i;
    if (!isalpha(label[0]))
        return 0;
    for (i = 1; label[i] != '\0'; i++) {
        if (!isalnum(label[i]))
            return 0;
    }
    if (get_operation(label) != NULL)
        return 0;
    return 1;
}

int is_label_duplicate(const char *label) {
    int i;
    for (i = 0; i < symbol_count; i++) {
        if (strcmp(symbol_table[i].name, label) == 0) {
            return 1;
        }
    }
    return 0;
}

void add_symbol(const char *name, int address, SymbolType type, int is_entry,
                int is_external) {
    if (symbol_count < MAX_SYMBOLS) {
        strncpy(symbol_table[symbol_count].name, name, MAX_LABEL_LENGTH);
        symbol_table[symbol_count].address = address;
        symbol_table[symbol_count].type = type;
        symbol_table[symbol_count].is_entry = is_entry;
        symbol_table[symbol_count].is_external = is_external;
        symbol_count++;
    }
}

void add_code_row(int address) {
    if (code_count < MAX_CODE_ROWS) {
        code_table[code_count].address = address;
        strcpy(code_table[code_count].binary, "000000000000000000000000");
        code_count++;
    }
}

int is_valid_register(const char *operand) {
    int result = operand[0] == 'r' && isdigit(operand[1]) && operand[2] == '\0' &&
                 operand[1] >= '0' && operand[1] <= '7';
    return result;
}

int is_valid_number(const char *s) {
    if (*s == '-' || *s == '+')
        s++;
    if (!*s)
        return 0;
    while (*s) {
        if (!isdigit(*s))
            return 0;
        s++;
    }
    return 1;
}

int looks_like_register(const char *operand) { return operand[0] == 'r'; }

int count_instruction_words(const Operation *op, char *operands_line,
                            const int line_number) {
    int count = 1;
    const char *src = NULL;
    const char *dest = NULL;
    const char *extraOp = NULL;
    AddressingMode src_mode = -1, dest_mode = -1;

    if (!op)
        return 0;

    if (op->operand_count == 0 && operands_line[0] != '\0') {
        return 1;
    }
    if (op->operand_count == 1) {
        printf("op->name: %s\n", op->name);
        dest = strtok(operands_line, ", \t");
        extraOp = strtok(NULL, ", \t");

        printf("dest: %s, extraOp: %s \n", dest, extraOp);

        if (extraOp != NULL) {
            report_first_pass_error(line_number, "Too many operands for instruction.");
        }
        if (dest != NULL) {
            while (isspace((unsigned char)*dest))
                dest++;
            dest_mode = get_addressing_mode(dest);
            if (looks_like_register(dest)) {
                if (!is_valid_register(dest)) {
                    report_first_pass_error(line_number, "Invalid register name.");
                }
            }
            if (dest_mode != REGISTER)
                count += 1;
        }
    } else if (op->operand_count == 2) {
        printf("op->name: %s op Count %d\n", op->name, op->operand_count);

        src = strtok(operands_line, ",");
        dest = strtok(NULL, ",");
        if (src == NULL || dest == NULL) {
            report_first_pass_error(line_number, "operands are missing for instruction.");
        }
        if (src != NULL) {
            while (isspace((unsigned char)*src))
                src++;
            src_mode = get_addressing_mode(src);
            if (looks_like_register(src)) {
                if (!is_valid_register(src)) {
                    report_first_pass_error(line_number,
                                            "Invalid source operand register name.");
                }
            }
        }
        dest = strtok(NULL, " ");
        if (dest != NULL) {
            while (isspace((unsigned char)*dest))
                dest++;
            dest_mode = get_addressing_mode(dest);
            if (looks_like_register(dest)) {
                if (!is_valid_register(dest)) {
                    report_first_pass_error(line_number,
                                            "Invalid destination operand register name.");
                }
            }
        }

        if (src_mode == REGISTER && dest_mode == REGISTER) {
            count += 0;
        } else {
            if (src_mode != REGISTER && src_mode != -1)
                count += 1;
            if (dest_mode != REGISTER && dest_mode != -1)
                count += 1;
        }
    }

    return count;
}

void first_pass(const char *filename) {
    FILE *file;
    char line[MAX_LINE_LENGTH];
    char label[MAX_LABEL_LENGTH];
    char *command, *token, *operands;
    char operands_copy[MAX_LINE_LENGTH];
    const Operation *op;
    int words;
    int i;
    int line_number = 0;

    file = fopen(filename, "r");
    if (!file) {
        printf("Error: cannot open file %s\n", filename);
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        line_number++;
        trim_leading_spaces(line);
        if (is_comment_or_empty(line))
            continue;

        label[0] = '\0';
        if (strchr(line, ':')) {
            sscanf(line, "%[^:]", label);
            if (!is_valid_label(label)) {
                report_first_pass_error(line_number, "Invalid label name.");
                continue;
            }
            if (is_label_duplicate(label)) {
                report_first_pass_error(line_number, "Duplicate label definition.");
                continue;
            }
            memmove(line, strchr(line, ':') + 1, strlen(strchr(line, ':')));
            trim_leading_spaces(line);
        }

        command = strtok(line, " \t\n");
        if (!command)
            continue;

        if (strcmp(command, ".data") == 0 || strcmp(command, ".string") == 0 ||
            strcmp(command, ".extern") == 0 || strcmp(command, ".entry") == 0) {
            continue;
        }

        op = get_operation(command);
        if (!op) {
            report_first_pass_error(line_number, "Unknown instruction.");
            continue;
        }

        if (label[0])
            add_symbol(label, IC, CODE, 0, 0);

        operands = strtok(NULL, "\n");
        if (operands != NULL)
            strncpy(operands_copy, operands, MAX_LINE_LENGTH);
        else
            operands_copy[0] = '\0';

        words = count_instruction_words(op, operands_copy, line_number);
        for (i = 0; i < words; i++) {
            add_code_row(IC);
            IC++;
        }
    }

    ICF = IC;
    rewind(file);

    while (fgets(line, sizeof(line), file)) {
        line_number++;
        trim_leading_spaces(line);
        if (is_comment_or_empty(line))
            continue;

        label[0] = '\0';
        if (strchr(line, ':')) {
            sscanf(line, "%[^:]", label);
            memmove(line, strchr(line, ':') + 1, strlen(strchr(line, ':')));
            trim_leading_spaces(line);
        }

        command = strtok(line, " \t\n");
        if (!command)
            continue;

        if (strcmp(command, ".data") == 0) {
            if (label[0])
                add_symbol(label, ICF + DC, DATA, 0, 0);
            token = strtok(NULL, ",\n");
            while (token) {
                trim_leading_spaces(token);
                if (!isdigit(token[0]) && !(token[0] == '-' && isdigit(token[1]))) {
                    report_first_pass_error(line_number,
                                            "Invalid number in .data directive.");
                }
                add_code_row(ICF + DC);
                DC++;
                token = strtok(NULL, ",\n");
            }
        } else if (strcmp(command, ".string") == 0) {
            if (label[0])
                add_symbol(label, ICF + DC, DATA, 0, 0);
            token = strtok(NULL, "\"");
            if (token) {
                for (i = 0; i < (int) strlen(token); i++) {
                    add_code_row(ICF + DC);
                    DC++;
                }
                add_code_row(ICF + DC);
                DC++;
            }
        } else if (strcmp(command, ".extern") == 0) {
            token = strtok(NULL, " \t\n");
            if (token)
                add_symbol(token, 0, EXTERNAL, 0, 1);
        }
    }

    fclose(file);
    DCF = DC;

    printf("\nSymbol Table:\n");
    for (i = 0; i < symbol_count; i++) {
        printf("%s -> %d [%s]\n", symbol_table[i].name, symbol_table[i].address,
               symbol_table[i].type == CODE
                   ? "CODE"
                   : symbol_table[i].type == DATA
                         ? "DATA"
                         : symbol_table[i].type == EXTERNAL
                               ? "EXTERN"
                               : "ENTRY");
    }

    printf("\nCode Table (ICF = %d, DCF = %d):\n", ICF, DCF);
    for (i = 0; i < code_count; i++) {
        printf("%04d %s\n", code_table[i].address, code_table[i].binary);
    }
}
