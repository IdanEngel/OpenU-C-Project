/* symbol_table.c */

#include <string.h>
#include "../Header_Files/symbol_table.h"

int add_symbol(Symbol *table, int *count, const char *name, int address, LabelType type) {
    int i;

    /* Check for duplicate label */
    for (i = 0; i < *count; i++) {
        if (strcmp(table[i].name, name) == 0) {
            return 0; /* Already exists */
        }
    }

    /* Add new symbol */
    strncpy(table[*count].name, name, MAX_LABEL_LENGTH);
    table[*count].name[MAX_LABEL_LENGTH] = '\0'; /* Safety null */
    table[*count].address = address;
    table[*count].type = type;

    (*count)++;
    return 1;
}

int find_symbol(Symbol *table, int count, const char *name) {
    int i;
    for (i = 0; i < count; i++) {
        if (strcmp(table[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}
