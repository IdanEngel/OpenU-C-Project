/* Placeholder comment */
#include "../Header_Files/instruction_tables.h"
#include "../Header_Files/utils.h"
#include "../Header_Files/codegen.h"
#include "../Header_Files/assembler.h"
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LABEL_LENGTH 31
#define MAX_LINE_LENGTH 31
#define MAX_SYMBOLS 100
#define MAX_CODE_ROWS 500


Symbol symbol_table[MAX_SYMBOLS];
int symbol_count = 0;
CodeRow code_table[MAX_CODE_ROWS];
unsigned int code[MAX_CODE_ROWS];
int code_count = 0;
int IC = 100;
int DC = 0;
int ICF = 0;
int DCF = 0;

int is_comment_or_empty(const char *line) {
    return line[0] == ';' || line[0] == '\0';
}



int count_operands(const char *operands_line) {
    int count = 0;
    char temp[MAX_LINE_LENGTH];
    char *token;

    strncpy(temp, operands_line, MAX_LINE_LENGTH);
    token = strtok(temp, ", \t");

    while (token != NULL) {
        count++;
        token = strtok(NULL, ", \t");
    }

    return count;
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
    const int result = operand[0] == 'r' && isdigit(operand[1]) && operand[2] == '\0' &&
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
    char operands_copy[MAX_LINE_LENGTH];
    /*const char *operands;*/


    AddressingMode src_mode = -1, dest_mode = -1;
    if (!op)
        return 0;

    if (op->operand_count == 1) {
        dest = strtok(operands_line, ", \t");
        extraOp = strtok(NULL, ", \t");
        if (extraOp != NULL) {
            report_errors(filename, line_number, "Too many operands for instruction.");
        }
        if (dest != NULL) {
            while (isspace((unsigned char)*dest))
                dest++;
            dest_mode = get_addressing_mode(dest);
            if (looks_like_register(dest)) {
                if (!is_valid_register(dest)) {
                    report_errors(filename, line_number, "Invalid register name.");
                }
            }
            if (dest_mode != REGISTER)
                count += 1;
        }
    } else if (op->operand_count == 2) {
        const int op_count = op->operand_count;
        char operand_copy[MAX_LINE_LENGTH];
        char operand_split[MAX_LINE_LENGTH];
        strncpy(operand_copy, operands_line, MAX_LINE_LENGTH);
        strncpy(operand_split, operands_line, MAX_LINE_LENGTH);
        /*operands = strtok(NULL, ", \t");*/
        strncpy(operands_copy, operands_line, MAX_LINE_LENGTH);
        if (count_operands(operands_line) == op_count && strchr(operand_copy, ',') == NULL && strchr(operand_copy, ' ')
            != NULL) {
            report_errors(filename, line_number, "Missing comma between operands.");
            printf("strchr-> %s\n", strchr(operand_copy, ' '));
            src = strtok(operand_copy, " ");
            printf("operands_line-> %s\n", src);
            dest = strtok(NULL, " ");
            if (looks_like_register(src) && !is_valid_register(src)) {
                report_errors(filename, line_number, "Invalid source operand register name.");
            } else if (looks_like_register(dest) && !is_valid_register(dest)) {
                report_errors(filename, line_number, "Invalid destination operand register name.");
            }
        } else {
            char *first = strtok(operands_copy, " ,\t");
            char *second = strtok(NULL, " ,\t");
            char *extra = strtok(NULL, " ,\t");

            if (!first || !second) {
                report_errors(filename, line_number, "Too few operands for instruction.");
            } else if (extra) {
                report_errors(filename, line_number, "Too many operands for instruction.");
            } else {
                src = strtok(operands_line, ",");
                if (src != NULL) {
                    while (isspace((unsigned char)*src))
                        src++;
                    src_mode = get_addressing_mode(src);
                    if (looks_like_register(src)) {
                        if (!is_valid_register(src)) {
                            report_errors(filename, line_number, "Invalid source operand register name.");
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
                            report_errors(filename, line_number,
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

void binary_to_string(unsigned int word, char *out) {
    int i;
    for (i = 0; i < 24; i++) {
        out[23 - i] = (word & (1 << i)) ? '1' : '0';
    }
    out[24] = '\0';
}

void negative_int_to_binary(short value, char *binary_out) {
    int i;
    unsigned int uval = (unsigned int) ((int) value & 0xFFFFFF); /* 24 bits signed */
    for (i = 23; i >= 0; i--) {
        binary_out[23 - i] = ((uval >> i) & 1) ? '1' : '0';
    }
    binary_out[24] = '\0';
}

void first_pass(const char *filename) {
    FILE *file;
    char line[MAX_LINE_LENGTH], label[MAX_LABEL_LENGTH], *command, *token, *operands, operands_copy[MAX_LINE_LENGTH], *arg_token;
    const Operation *op;
    Line temp_line;
    int i, line_number = 0, data_index = 0, start_ic;
    short value;

    file = fopen(filename, "r");
    if (!file) {
        printf("Error: cannot open file %s\n", filename);
        error_flag = 1;
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
                report_errors(filename, line_number, "Invalid label name.");
                continue;
            }
            if (is_label_duplicate(label)) {
                report_errors(filename, line_number, "Duplicate label definition.");
                continue;
            }
            memmove(line, strchr(line, ':') + 1, strlen(strchr(line, ':')));
            trim_leading_spaces(line);
        }

        command = strtok(line, " \t\n");
        if (!command) continue;

        if (strcmp(command, ".data") == 0) {
            if (label[0])
                add_symbol(label, IC + DC, DATA, 0, 0);
            token = strtok(NULL, ",\n");
            while (token) {
                trim_leading_spaces(token);
                value = (short) atoi(token);
                if (code_count < MAX_CODE_ROWS) {
                    code_table[code_count].address = IC + data_index;
                    if (value < 0) {
                        negative_int_to_binary(value, code_table[code_count].binary);
                    } else {
                        binary_to_string((unsigned short) value, code_table[code_count].binary);
                    }
                    code_count++;
                    data_index++;
                }
                DC++;
                token = strtok(NULL, ",\n");
            }
            continue;
        } else if (strcmp(command, ".string") == 0) {
            if (label[0])
                add_symbol(label, IC + DC, DATA, 0, 0);
            token = strtok(NULL, "\"");
            if (token) {
                for (i = 0; i < (int) strlen(token); i++) {
                    if (code_count < MAX_CODE_ROWS) {
                        code_table[code_count].address = IC + data_index;
                        binary_to_string((unsigned short) token[i], code_table[code_count].binary);
                        code_count++;
                        data_index++;
                        DC++;
                    }
                }
                if (code_count < MAX_CODE_ROWS) {
                    code_table[code_count].address = IC + data_index;
                    binary_to_string(0, code_table[code_count].binary);
                    code_count++;
                    data_index++;
                    DC++;
                }
            }
            continue;
        } else if (strcmp(command, ".extern") == 0) {
            token = strtok(NULL, " \t\n");
            if (token)
                add_symbol(token, 0, EXTERNAL, 0, 1);
            continue;
        } else if (strcmp(command, ".entry") == 0) {
            continue;
        }

        op = get_operation(command);
        if (!op) {
            report_errors(filename, line_number, "Unknown instruction.");
            continue;
        }

        if (label[0]) add_symbol(label, IC, CODE, 0, 0);

        operands = strtok(NULL, "\n");
        if (operands != NULL)
            strncpy(operands_copy, operands, MAX_LINE_LENGTH);
        else
            operands_copy[0] = '\0';


        memset(&temp_line, 0, sizeof(Line));
        temp_line.opcode = command;
        temp_line.arg_count = 0;

        arg_token = strtok(operands_copy, ", \t");
        while (arg_token && temp_line.arg_count < 2) {
            temp_line.args[temp_line.arg_count++] = arg_token;
            arg_token = strtok(NULL, ", \t");
        }

        start_ic = IC;
        if (!count_instruction_words(op, operands, filename, line_number)) {
            report_errors(filename, line_number, "Operand validation failed.");
            continue;
        }

        if (!encode_instruction_binary(temp_line, code, &IC)) {
            report_errors(filename, line_number, "Failed to encode instruction.");
            continue;
        }

        for (i = start_ic; i < IC; i++) {
            if (code_count < MAX_CODE_ROWS) {
                code_table[code_count].address = i;
                binary_to_string(code[i], code_table[code_count].binary);
                code_count++;
            }
        }
    }

    ICF = IC;
    DCF = DC;

    fclose(file);
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
