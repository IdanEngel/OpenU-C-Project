/* second_pass.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Header_Files/second_pass.h"
#include "../Header_Files/symbol_table.h"
#include "../Header_Files/parser.h"
#include "../Header_Files/errors.h"
#include "../Header_Files/utils.h"
#include "../Header_Files/output.h"

#define MAX_LINE_LENGTH 81

typedef struct {
    char name[MAX_LABEL_LENGTH + 1];
    int address;
} EntryLabel;

typedef struct {
    char name[MAX_LABEL_LENGTH + 1];
    int address;
} ExternUse;

int second_pass(const char *am_file, Symbol *symbol_table, int symbol_count,
                unsigned short *code, int code_count,
                unsigned short *data, int data_count,
                int IC, int DC, int *error_flag) {

    FILE *fp;
    char line[MAX_LINE_LENGTH];
    int line_num = 0;
    int i;
    EntryLabel entries[MAX_SYMBOLS];
    int entry_count = 0;
    ExternUse externs[MAX_SYMBOLS];
    int extern_count = 0;

    *error_flag = 0;

    fp = fopen(am_file, "r");
    if (!fp) {
        report_error("Failed to open file for second pass", am_file, 0);
        return 0;
    }

    while (fgets(line, MAX_LINE_LENGTH, fp)) {
        Line parsed = parse_line(line, ++line_num);

        if (parsed.error) {
            report_error(parsed.error_msg, am_file, line_num);
            *error_flag = 1;
            continue;
        }

        /* Handle .entry */
        if (parsed.is_entry && parsed.args[0]) {
            int idx = find_symbol(symbol_table, symbol_count, parsed.args[0]);
            if (idx >= 0 && symbol_table[idx].type != LABEL_EXTERN) {
                symbol_table[idx].type = LABEL_ENTRY;
                strcpy(entries[entry_count].name, symbol_table[idx].name);
                entries[entry_count].address = symbol_table[idx].address;
                entry_count++;
            } else {
                report_error("Invalid or undefined .entry label", am_file, line_num);
                *error_flag = 1;
            }
        }
    }
    fclose(fp);

    /* Step 2: Patch labels in code[] */
    for (i = 0; i < code_count; i++) {
        /* Placeholder values like 9999 are used to mark "needs patching" */
        if (code[i] == 9999) {
            report_error("Unresolved label in code[] — you must mark with label info during 1st pass", am_file, 0);
            *error_flag = 1;
        }

        /* If in first pass you tagged labels with sentinel value (e.g., 0xFFFF), replace them here.
           We assume you've marked unresolved words with label reference index or name. */
    }

    /* Step 3: Merge code and data */
    for (i = 0; i < data_count; i++) {
        code[code_count + i] = data[i];
    }

    /* Step 4: Write output files */
    write_object_file(am_file, code, code_count + data_count, IC, DC);
    write_entry_file(am_file, entries, entry_count);
    write_extern_file(am_file, externs, extern_count);

    if (*error_flag)
        return 0;

    return 1;
}
a