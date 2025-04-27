#include "../Header_Files/symbol_table.h"
#include <string.h>

Symbol symbol_table[MAX_SYMBOLS];
int symbol_count = 0;

CodeRow code_table[MAX_CODE_ROWS];
int code_count = 0;
int IC = 100;
int DC = 0;
int ICF = 0;
int DCF = 0;


/*
 * Add a new symbol to the symbol table.
 * Parameters:
 * - name: name of the symbol.
 * - address: memory address associated with the symbol.
 * - type: type of symbol (CODE, DATA, EXTERNAL).
 * - is_entry: whether the symbol is declared as an entry point (1 = yes, 0 = no).
 * - is_external: whether the symbol is external (1 = yes, 0 = no).
 */
void add_symbol(const char *name, int address, SymbolType type, int is_entry, int is_external) {
    if (symbol_count >= MAX_SYMBOLS) return;

    strncpy(symbol_table[symbol_count].name, name, MAX_LABEL_LENGTH);
    symbol_table[symbol_count].name[MAX_LABEL_LENGTH - 1] = '\0';
    symbol_table[symbol_count].address = address;
    symbol_table[symbol_count].type = type;
    symbol_table[symbol_count].is_entry = is_entry;
    symbol_table[symbol_count].is_external = is_external;

    symbol_count++;
}

/*
 * Find a symbol in the symbol table.
 * Parameters:
 * - table: the symbol table array.
 * - count: number of symbols currently stored.
 * - name: name of the symbol to find.
 * Returns:
 * - The index of the symbol if found, or -1 if not found.
 */
int find_symbol(Symbol *table, int count, const char *name) {
    int i;
    for (i = 0; i < count; i++) {
        if (strcmp(table[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}
