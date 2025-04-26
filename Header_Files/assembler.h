/* assembler.h */

#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#define NULL ((void *)0)
void run_assembler(char *file_name);
extern int error_flag;
void report_errors(const char *filename, int line_number, const char *message);

#endif
