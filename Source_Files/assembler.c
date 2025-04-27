#include <stdio.h>
#include <string.h>
#include "../Header_Files/assembler.h"
#include "../Header_Files/first_pass.h"
#include "../Header_Files/symbol_table.h"

#define MAX_LINE_LENGTH 100
#define MAX_MACRO_COUNT 50
#define MAX_MACRO_LENGTH 1000
#define MAX_LABEL_LENGTH 31

/* Forward declarations */
void preprocess_macros(const char *input_filename, const char *output_filename);

void first_pass(const char *filename);
void second_pass(const char *filename);
void create_output_files(const char *basename);

int error_flag = 0;


void report_errors(const char *filename, const int line_number, const char *message) {
    printf("\033[1;31m[File %s, Line %d] Error: \033[0m %s\n", filename, line_number, message);
    error_flag = 1;
}


void reset_global_data(void) {
    symbol_count = 0;
    code_count = 0;
    extern_use_count = 0;
    IC = 100;
    DC = 0;
    error_flag = 0;
}

#include <stdlib.h>
#include <string.h>
#include <stdio.h>




void process_file(const char *input_filename) {
    char am_filename[100];
    char base_name[100];
    char *dot;

    /* copy the base name */
    strncpy(base_name, input_filename, sizeof(base_name));
    base_name[sizeof(base_name) - 1] = '\0';

    /* deleting .as files ending to create .am file ending */
    dot = strrchr(base_name, '.');
    if (dot && strcmp(dot, ".as") == 0) {
        *dot = '\0'; /* מסיר את הסיומת */
    } else {
        printf("Error: file %s does not have .as extension\n", input_filename);
        return;
    }

    printf("\nProcessing file: %s\n", input_filename);

    /* Macro file name checking */
    if (strlen(base_name) + 3 >= sizeof(am_filename)) {
        printf("Error: File name too long.\n");
        return;
    }
    strcpy(am_filename, base_name);
    strcat(am_filename, ".am");
    printf("\033[1;32mMacro Preprocessing:\033[0m creating file: %s\n", am_filename);
    preprocess_macros(input_filename, am_filename);

    /* First Pass */
    printf("\033[1;32mFirst And Second Pass:\033[0m scanning file: %s\n", am_filename);
    first_pass(am_filename);
    /* Second Pass */
    second_pass(am_filename);

    /* Checking if any errors occurred to create output files */
    if (!error_flag) {
        printf("\033[1;32mScans ran successfully, Creating output files...\033[0m\n");
        create_output_files(base_name);
    } else {
        printf("\033[1;31mErrors detected!\033[0m No output files created.\n");
    }

    /* global variables reset before next file read */
    reset_global_data();
}


int main(const int argc, char *argv[]) {
    int i;

    if (argc < 2) {
        printf("Usage: %s file1.as [file2.as ...]\n", argv[0]);
        return 1;
    }

    for (i = 1; i < argc; i++) {
        process_file(argv[i]);
    }

    return 0;
}
