#ifndef FIRST_PASS_H
#define FIRST_PASS_H


extern int code_count;
extern int IC;
extern int DC;
extern int ICF;
extern int DCF;

/* Builds the symbol table from the expanded assembly file (.am) */
void first_pass(const char *filename);
void add_code_row(int address);

#endif
