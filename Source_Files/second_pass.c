#include <stdio.h>
#include <string.h>
#include "../Header_Files/instruction_tables.h"
#include "../Header_Files/symbol_table.h"
#include "../Header_Files/utils.h"
#include "../Header_Files/assembler.h"

#define MAX_LINE_LENGTH 31




void second_pass(const char *filename) {
    FILE *file = fopen(filename, "r");
    char line[MAX_LINE_LENGTH], *bin, *command, *operands;
    int line_number = 0, code_index = 0, k, word_offset;
    const Operation *op;
    unsigned int value;
    char error_msg[100];


    if (!file) {
        printf("Error: cannot open file %s\n", filename);
        error_flag = 1;
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        line_number++;
        trim_leading_spaces(line);
        if (line[0] == ';' || line[0] == '\0') continue;

        if (strchr(line, ':')) {
            memmove(line, strchr(line, ':') + 1, strlen(strchr(line, ':')));
            trim_leading_spaces(line);
        }

        command = strtok(line, " \t\n");
        if (!command) continue;

        op = get_operation(command);
        if (!op) continue; /* skip .data, .string, etc */

        operands = strtok(NULL, "\n");
        word_offset = 1; /* Start after the instruction word */


        if (operands) {
            char *token = strtok(operands, ", ");
            while (token) {
                trim_leading_spaces(token);

                if (token[0] != '#' && !is_register(token)) {
                    const char *label_name = (token[0] == '&') ? token + 1 : token;
                    int j;
                    int found = 0;
                    for (j = 0; j < symbol_count; j++) {
                        if (strcmp(symbol_table[j].name, label_name) == 0) {
                            found = 1;
                            bin = code_table[code_index + word_offset].binary;
                            if (token[0] == '&') {
                                /* Calculate relative offset from next instruction */
                                value = symbol_table[j].address - (code_table[code_index].address);
                                for (k = 20; k >= 0; k--) {
                                    bin[20 - k] = ((value >> k) & 1) + '0';
                                }
                                strcpy(bin + 21, "100"); /* ARE = 100 for relative */
                            } else if (symbol_table[j].is_external) {
                                strcpy(bin, "000000000000000000000001");
                                strcpy(extern_use_table[extern_use_count].label, symbol_table[j].name);
                                extern_use_table[extern_use_count].address = code_table[code_index + word_offset].
                                        address;
                                extern_use_count++;
                            } else if (symbol_table[j].is_entry) {
                            } else {
                                value = symbol_table[j].address;
                                for (k = 20; k >= 0; k--) {
                                    bin[20 - k] = ((value >> k) & 1) + '0';
                                }
                                strcpy(bin + 21, "010");
                            }
                            break;
                        }
                    }
                    if (found == 0 && looks_like_register(token) == 0) {
                        sprintf(error_msg, "Undefined label name: %s", label_name);
                        report_errors(filename, line_number, error_msg);
                        error_flag = 1;
                    }
                    word_offset++;
                } else if (token[0] == '#') {
                    word_offset++;
                } /* Do not increment word_offset for registers alone */

                token = strtok(NULL, ", ");
            }

            code_index += word_offset;
        } else {
            code_index++;
        }
    }

    fclose(file);

}


void create_output_files(const char *basename) {
    FILE *ob_file, *ext_file, *ent_file, *input_file;
    char ob_name[100], ext_name[100], ent_name[100], am_name[100];
    char line[MAX_LINE_LENGTH];
    int i, line_number = 0;
    int found;
    char *command;
    char error_msg[100];

    /* Process .entry directives from .am file */
    sprintf(am_name, "%s.am", basename);
    input_file = fopen(am_name, "r");
    if (input_file) {
        while (fgets(line, sizeof(line), input_file)) {
            line_number++;
            command = strtok(line, " \t\n");
            if (command && strcmp(command, ".entry") == 0) {
                char *entry_label = strtok(NULL, " \t\n");
                if (entry_label) {
                    found = 0;
                    for (i = 0; i < symbol_count; i++) {
                        if (strcmp(symbol_table[i].name, entry_label) == 0) {
                            symbol_table[i].is_entry = 1;
                            found = 1;
                            break;
                        }
                    }
                    if (!found) {
                        sprintf(error_msg, "Error: .entry label %s not found in symbol table", entry_label);
                        report_errors(basename, line_number, error_msg);
                    }
                }
            }
        }
        fclose(input_file);
    }

    /* Create .ob file */
    sprintf(ob_name, "Output_Files/%s.ob", basename);
    ob_file = fopen(ob_name, "w");
    if (!ob_file) {
        printf("Error: Failed to create output file %s\n", ob_name);
        return;
    }
    fprintf(ob_file, "     %d %d\n", ICF - 100, DCF);
    for (i = 0; i < code_count; i++) {
        unsigned int value = 0;
        int j;
        for (j = 0; j < 24; j++) {
            value <<= 1;
            if (code_table[i].binary[j] == '1') {
                value |= 1;
            }
        }
        fprintf(ob_file, "%07d %06x\n", code_table[i].address, value);
    }
    fclose(ob_file);

    /* Create .ext file */
    sprintf(ext_name, "Output_Files/%s.ext", basename);
    ext_file = fopen(ext_name, "w");
    if (ext_file) {
        for (i = 0; i < extern_use_count; i++) {
            fprintf(ext_file, "%s %07d\n", extern_use_table[i].label, extern_use_table[i].address);
        }
        fclose(ext_file);
    }

    /* Create .ent file */
    sprintf(ent_name, "Output_Files/%s.ent", basename);
    ent_file = fopen(ent_name, "w");
    if (ent_file) {
        for (i = 0; i < symbol_count; i++) {
            if (symbol_table[i].is_entry) {
                fprintf(ent_file, "%s %07d\n", symbol_table[i].name, symbol_table[i].address);
            }
        }
        fclose(ent_file);
    }
}
