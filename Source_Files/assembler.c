#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Header_Files/assembler.h"

#define MAX_LINE_LENGTH 100
#define MAX_MACRO_COUNT 50
#define MAX_MACRO_LENGTH 1000
#define MAX_LABEL_LENGTH 31

/* Forward declarations */
void preprocess_macros(const char *output_filename, const char *input_filename);

void first_pass(const char *filename);

void second_pass(const char *filename);

void create_output_files(const char *basename);

int error_flag = 0;

void report_errors(const char *filename, int line_number, const char *message) {
    printf("[File %s, Line %d] Error: %s\n", filename, line_number, message);
    error_flag = 1;
}

int main(int argc, char *argv[]) {
    char *dot;
    char basename[100];

    if (argc != 3) {
        printf("Usage: %s <input file .as> <output file .am>\n", argv[0]);
        return 1;
    }

    preprocess_macros(argv[2], argv[1]);
    printf("Macro expansion complete. Output saved to %s\n", argv[1]);

    first_pass(argv[1]);
    printf("First pass complete. Output saved to %s\n", argv[1]);
    second_pass(argv[1]);

    /* remove extension (.am) and pass base name to output */
    strcpy(basename, argv[1]);
    dot = strrchr(basename, '.');
    if (dot) *dot = '\0';

    if (error_flag == 0) {
        printf("No errors detected. Creating output files...\n");
        create_output_files(basename);
    } else {
        printf("Errors detected. No output files created.\n");
    }

    return 0;
}

/* (rest of the functions follow here...) */
