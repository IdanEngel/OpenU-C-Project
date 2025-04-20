#ifndef GLOBALS_H
#define GLOBALS_H

#include "../Header_Files/instruction_tables.h"

#define MAX_SYMBOLS 100
#define MAX_CODE_ROWS 500
#define MAX_LABEL_LENGTH 31

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

/* Declare as extern */
extern Symbol symbol_table[MAX_SYMBOLS];
extern int symbol_count;

extern CodeRow code_table[MAX_CODE_ROWS];
extern int code_count;

extern int IC, DC, ICF, DCF;

#endif /* GLOBALS_H */
