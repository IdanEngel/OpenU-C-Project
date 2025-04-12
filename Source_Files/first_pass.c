/* first_pass.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Header_Files/first_pass.h"
#include "../Header_Files/symbol_table.h"
#include "../Header_Files/errors.h"
#include "../Header_Files/utils.h"
#include "../Header_Files/../Header_Files/parser.h"
#include "../Header_Files/codegen.h"

#define MAX_SYMBOLS 1000
#define MAX_CODE 1000
#define MAX_DATA 1000

int first_pass(const char *am_file, int *error_flag) {
    FILE *fp;
    char line[MAX_LINE_LENGTH];
    int line_num = 0;
    int IC = 100, DC = 0;
    Symbol symbol_table[MAX_SYMBOLS];
    int symbol_count = 0;
    unsigned short code[MAX_CODE];
    int code_count = 0;
    unsigned short data[MAX_DATA];
    int data_count = 0;

    *error_flag = 0;

    fp = fopen(am_file, "r");
    if (!fp) {
        report_error("Failed to open .am file", am_file, 0);
        return 0;
    }

    while (fgets(line, MAX_LINE_LENGTH, fp)) {
        Line parsed;
        line_num++;

        /* Parse the line into label/opcode/args */
        parsed = parse_line(line, line_num);
        if (parsed.error) {
            report_error(parsed.error_msg, am_file, line_num);
            *error_flag = 1;
            continue;
        }

        /* If the line defines a label */
        if (parsed.label != NULL) {
            if (!add_symbol(symbol_table, &symbol_count, parsed.label,
                            parsed.is_data ? DC : IC,
                            parsed.is_data ? LABEL_DATA : LABEL_CODE)) {
                report_error("Duplicate label", am_file, line_num);
                *error_flag = 1;
                continue;
            }
        }

        /* Handle instructions */
        if (parsed.is_data) {
            if (!handle_data_instruction(parsed, data, &DC)) {
                report_error("Invalid .data/.string", am_file, line_num);
                *error_flag = 1;
            }
        } else if (parsed.opcode != NULL) {
            if (!encode_instruction_stub(parsed, code, &IC)) {
                report_error("Invalid instruction line", am_file, line_num);
                *error_flag = 1;
            }
        } else if (parsed.is_extern) {
            if (!add_symbol(symbol_table, &symbol_count, parsed.args[0], 0, LABEL_EXTERN)) {
                report_error("Duplicate extern label", am_file, line_num);
                *error_flag = 1;
            }
        }
    }

    fclose(fp);

    if (*error_flag)
        return 0;

    /* Normally you’d store or return symbol_table, code, data for second pass */
    return 1;
}
