/* Placeholder comment */
#include "../Header_Files/instruction_tables.h"
#include "../Header_Files/utils.h"
#include "../Header_Files/first_pass.h"
#include "../Header_Files/symbol_table.h"
#include "../Header_Files/parser.h"
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100
#define MAX_LABEL_LENGTH 31
#define MAX_CODE_ROWS 500


int is_comment_or_empty(const char *line) {
    return line[0] == ';' || line[0] == '\0';
}

void report_first_pass_error(const char *filename, int line_number, const char *message) {
    printf("[File %s, Line %d] Error: %s\n", filename, line_number, message);
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

/*
void add_symbol(const char *name, const int address, SymbolType type, const int is_entry,
                const int is_external) {
    if (symbol_count < MAX_SYMBOLS) {
        strncpy(symbol_table[symbol_count].name, name, MAX_LABEL_LENGTH);
        symbol_table[symbol_count].address = address;
        symbol_table[symbol_count].type = type;
        symbol_table[symbol_count].is_entry = is_entry;
        symbol_table[symbol_count].is_external = is_external;
        symbol_count++;
    }
} */


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
                            const char *filename, const int line_number) {
    int count = 1;
    const char *src = NULL;
    const char *dest = NULL;
    const char *extraOp = NULL;
    char *only_operands;

    AddressingMode src_mode = -1, dest_mode = -1;

    if (!op)
        return 0;

    if (op->operand_count == 1) {
        dest = strtok(operands_line, ", \t");
        extraOp = strtok(NULL, ", \t");
        if (extraOp != NULL) {
            report_first_pass_error(filename, line_number, "Too many operands for instruction.");
        }
        if (dest != NULL) {
            while (isspace((unsigned char)*dest))
                dest++;
            dest_mode = get_addressing_mode(dest);
            if (looks_like_register(dest)) {
                if (!is_valid_register(dest)) {
                    report_first_pass_error(filename, line_number, "Invalid register name.");
                }
            }
            if (dest_mode != REGISTER)
                count += 1;
        }
    } else if (op->operand_count == 2) {
        char operand_copy[MAX_LINE_LENGTH];
        char operands_copy[MAX_LINE_LENGTH];
        strncpy(operand_copy, operands_line, MAX_LINE_LENGTH);
        only_operands = strtok(NULL, " \n");
        strncpy(operands_copy, operands_line, MAX_LINE_LENGTH);
        if (only_operands != NULL && strlen(only_operands) > 0 && strchr(operand_copy, ',') == NULL &&
            strchr(operand_copy, ' ') != NULL) {
            report_first_pass_error(filename, line_number, "Missing comma between operands.");
        } else {
            char *first = strtok(operands_copy, " ,\t");
            char *second = strtok(NULL, " ,\t");
            char *extra = strtok(NULL, " ,\t");

            if (!first || !second) {
                report_first_pass_error(filename, line_number, "Too few operands for instruction.");
            } else if (extra) {
                report_first_pass_error(filename, line_number, "Too many operands for instruction.");
            } else {
                src = strtok(operands_line, ",");
                if (src != NULL) {
                    while (isspace((unsigned char)*src))
                        src++;
                    src_mode = get_addressing_mode(src);
                    if (looks_like_register(src)) {
                        if (!is_valid_register(src)) {
                            report_first_pass_error(filename, line_number, "Invalid source operand register name.");
                            return 1;
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
                            report_first_pass_error(filename, line_number,
                                                    "Invalid destination operand register name.");
                        }
                    }
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


void handle_instruction(const Operation *op, const char *operand_line, int address) {
    char operand_copy[MAX_LINE_LENGTH];
    char *src = NULL, *dest = NULL;
    AddressingMode src_mode = NONE, dest_mode = NONE;
    int src_reg = 0, dest_reg = 0;

    strncpy(operand_copy, operand_line, MAX_LINE_LENGTH);
    src = strtok(operand_copy, ",");
    dest = strtok(NULL, ",");

    trim_spaces(src);
    trim_spaces(dest);

    src_mode = get_addressing_mode(src);
    dest_mode = get_addressing_mode(dest);

    if (src_mode == REGISTER) src_reg = src[1] - '0';
    if (dest_mode == REGISTER) dest_reg = dest[1] - '0';

    add_binary_instruction(op->opcode, src_mode, dest_mode,
                           src_reg, dest_reg, op->funct, address);
}


void first_pass(const char *filename) {
    FILE *file;
    char line[MAX_LINE_LENGTH];
    char label[MAX_LABEL_LENGTH];
    char *command, *token;
    int line_number = 0;
    int i;

    file = fopen(filename, "r");
    if (!file) {
        printf("Error: cannot open file %s\n", filename);
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        line_number++;
        trim_leading_spaces(line);
        if (is_comment_or_empty(line)) continue;

        label[0] = '\0';
        if (strchr(line, ':')) {
            sscanf(line, "%[^:]", label);
            if (!is_valid_label(label)) {
                report_first_pass_error(filename, line_number, "Invalid label name.");
                continue;
            }
            if (is_label_duplicate(label)) {
                report_first_pass_error(filename, line_number, "Duplicate label definition.");
                continue;
            }
            memmove(line, strchr(line, ':') + 1, strlen(strchr(line, ':')));
            trim_leading_spaces(line);
        }

        command = strtok(line, " \t\n");
        if (!command) continue;

        if (strcmp(command, ".data") == 0) {
            int values[MAX_LINE_LENGTH], count = 0;
            token = strtok(NULL, ", \t\n");
            while (token) {
                values[count++] = atoi(token);
                token = strtok(NULL, ", \t\n");
            }
            encode_data_directive(label, values, count, &DC);
        } else if (strcmp(command, ".string") == 0) {
            token = strtok(NULL, "\"");
            encode_string_directive(label, token, &DC);
        } else if (strcmp(command, ".extern") == 0) {
            token = strtok(NULL, " \t\n");
            if (token) add_symbol(token, 0, EXTERNAL, 0, 1);
        } else if (strcmp(command, ".entry") == 0) {
            continue;
        } else {
            token = strtok(NULL, "\n");
            handle_instruction(command, token, filename, line_number, &IC);
        }

        memset(line, 0, sizeof(line));
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
                    report_first_pass_error(filename, line_number,
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
