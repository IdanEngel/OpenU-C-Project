/* Placeholder comment */
#include "../Header_Files/instruction_tables.h"
#include "../Header_Files/utils.h"
#include "../Header_Files/parser.h"
#include "../Header_Files/symbol_table.h"
#include "../Header_Files/first_pass.h"
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>





/*int is_comment_or_empty(const char *line) {
//    return line[0] == ';' || line[0] == '\0';
//}
//
//void report_first_pass_error(const char *filename, int line_number, const char *message) {
//    printf("[File %s, Line %d] Error: %s\n", filename, line_number, message);
//}
//
//int is_valid_label(const char *label) {
//    int i;
//    if (!isalpha(label[0])) return 0;
//    for (i = 1; label[i] != '\0'; i++) {
//        if (!isalnum(label[i])) return 0;
//    }
//    if (get_operation(label) != NULL) return 0;
//    return 1;
//}
//
//int is_label_duplicate(const char *label) {
//    int i;
//    for (i = 0; i < symbol_count; i++) {
//        if (strcmp(symbol_table[i].name, label) == 0) {
//            return 1;
//        }
//    }
//    return 0;
//}
//
//void add_symbol(const char *name, int address, SymbolType type, int is_entry, int is_external) {
//    if (symbol_count < MAX_SYMBOLS) {
//        strncpy(symbol_table[symbol_count].name, name, MAX_LABEL_LENGTH);
//        symbol_table[symbol_count].address = address;
//        symbol_table[symbol_count].type = type;
//        symbol_table[symbol_count].is_entry = is_entry;
//        symbol_table[symbol_count].is_external = is_external;
//        symbol_count++;
//    }
//}
*/
void add_code_row(int address) {
    if (code_count < MAX_CODE_ROWS) {
        code_table[code_count].address = address;
        strcpy(code_table[code_count].binary, "000000000000000000000000");
        code_count++;
    }
}
/*
//int is_valid_register(const char *operand) {
//    int result = operand[0] == 'r' && isdigit(operand[1]) && operand[2] == '\0' && operand[1] >= '0' && operand[1] <= '7';
//    return result;
//}
//
//int is_valid_number(const char *s) {
//    if (*s == '-' || *s == '+') s++;
//    if (!*s) return 0;
//    while (*s) {
//        if (!isdigit(*s)) return 0;
//        s++;
//    }
//    return 1;
//}
//
//int looks_like_register(const char *operand) {
//    return operand[0] == 'r';
//}

int get_expected_register_count(const char *opcode_name) {
    const Operation* op = get_operation(opcode_name);
    if (!op) return -1;
    return op->operand_count;
}*/

/* Convert an integer to a binary string representation of a given width */
void int_to_binary(int value, int width, char *output) {
    int i;
    for (i = width - 1; i >= 0; i--) {
        output[width - 1 - i] = ((value >> i) & 1) ? '1' : '0';
    }
    output[width] = '\0';
}

/* Build the binary instruction and insert it into the code table */
void add_binary_instruction(int opcode, AddressingMode src_mode, AddressingMode dest_mode,
                            int src_reg, int dest_reg, int funct, int address) {
    char bin_opcode[7], bin_src_mode[3], bin_dest_mode[3];
    char bin_src_reg[4], bin_dest_reg[4], bin_funct[6], ARE[] = "100";
    char binary[25];

    int_to_binary(opcode, 6, bin_opcode);
    int_to_binary(src_mode, 2, bin_src_mode);
    int_to_binary(src_reg, 3, bin_src_reg);
    int_to_binary(dest_mode, 2, bin_dest_mode);
    int_to_binary(dest_reg, 3, bin_dest_reg);
    int_to_binary(funct, 5, bin_funct);

    snprintf(binary, sizeof(binary), "%s%s%s%s%s%s%s",
             bin_opcode, bin_src_mode, bin_src_reg,
             bin_dest_mode, bin_dest_reg, bin_funct, ARE);

    binary[24] = '\0';
    add_code_row(address);
    strncpy(code_table[code_count - 1].binary, binary, 25);
}

/* Add binary line for symbol operand */
void add_symbol_operand(const char *symbol_name, int address) {
    char binary[25];
    char bin_address[22];
    const Symbol *sym = NULL;
    int i;

    for (i = 0; i < symbol_count; i++) {
        if (strcmp(symbol_table[i].name, symbol_name) == 0) {
            sym = &symbol_table[i];
            break;
        }
    }

    if (sym) {
        int_to_binary(sym->address, 21, bin_address);
        if (sym->is_external)
            snprintf(binary, sizeof(binary), "%s001", bin_address); /* E */
        else
            snprintf(binary, sizeof(binary), "%s010", bin_address); /* R */
    } else {
        snprintf(binary, sizeof(binary), "%021d010", 0); /* Default R until resolved */
    }

    binary[24] = '\0';
    add_code_row(address);
    strncpy(code_table[code_count - 1].binary, binary, 25);
}

/* Encode string directive as binary rows */
void encode_string_directive(const char *str, int base_address) {
    int i;
    char bin_value[22];
    char binary[25];
    for (i = 0; str[i] != '\0'; i++) {
        int_to_binary((int)str[i], 24, binary);
        binary[24] = '\0';
        add_code_row(base_address + i);
        strncpy(code_table[code_count - 1].binary, binary, 25);
    }
    int_to_binary(0, 24, binary);
    binary[24] = '\0';
    add_code_row(base_address + i);
    strncpy(code_table[code_count - 1].binary, binary, 25);
}

/* Encode .data directive values as binary rows */
void encode_data_directive(const int *values, int count, int base_address) {
    int i;
    char binary[25];
    for (i = 0; i < count; i++) {
        int_to_binary(values[i], 24, binary);
        binary[24] = '\0';
        add_code_row(base_address + i);
        strncpy(code_table[code_count - 1].binary, binary, 25);
    }
}
