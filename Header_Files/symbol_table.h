/* symbol_table.h */

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#define MAX_LABEL_LENGTH 31
#define MAX_SYMBOLS 1000

typedef enum {
    LABEL_CODE,
    LABEL_DATA,
    LABEL_EXTERN,
    LABEL_ENTRY
} LabelType;

typedef struct {
    char name[MAX_LABEL_LENGTH + 1]; /* 31 chars max + null terminator */
    int address;
    LabelType type;
} Symbol;

/**
 * @brief Adds a symbol to the symbol table.
 *
 * @param table Symbol table array.
 * @param count Pointer to the current count of symbols.
 * @param name Name of the symbol (label).
 * @param address Address value (IC or DC).
 * @param type Type of the label (code/data/extern).
 * @return 1 if success, 0 if duplicate.
 */
int add_symbol(Symbol *table, int *count, const char *name, int address, LabelType type);

/**
 * @brief Finds a symbol in the table by name.
 *
 * @param table Symbol table.
 * @param count Number of symbols.
 * @param name Name to look for.
 * @return Index of symbol if found, -1 if not.
 */
int find_symbol(Symbol *table, int count, const char *name);

#endif /* SYMBOL_TABLE_H */
