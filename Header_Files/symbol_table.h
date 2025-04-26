/* symbol_table.h */

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#define MAX_LABEL_LENGTH 31
#define MAX_SYMBOLS 100

typedef enum {
    LABEL_CODE,
    LABEL_DATA,
    LABEL_EXTERN,
    LABEL_ENTRY
} LabelType;


#define MAX_CODE_ROWS 500
#define MAX_LABEL_LENGTH 31
#define MAX_EXTERN_USES 100



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

typedef struct {
    char label[MAX_LABEL_LENGTH];
    int address;
} ExternUse;

/* Declare as extern */
extern Symbol symbol_table[MAX_SYMBOLS];
extern int symbol_count;

extern CodeRow code_table[MAX_CODE_ROWS];
extern int code_count;

extern ExternUse extern_use_table[MAX_EXTERN_USES];

extern int extern_use_count;
extern int IC, DC, ICF, DCF;
/**
 * @brief Adds a symbol to the symbol table.
 *
 * @param table Symbol table array.
 * @param count Pointer to the current count of symbols.
 * @param name Name of the symbol (label).
 * @param address Address value (IC or DC).
 * @param type Type of the label (code/data/extern).
 * @param is_entry
 * @param is_external
 * @return 1 if success, 0 if duplicate.
 */
void add_symbol(const char *name, int address, SymbolType type, int is_entry,
                int is_external);
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
