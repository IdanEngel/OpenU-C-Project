#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include "../Header_Files/assembler.h"
#include "../Header_Files/first_pass.h"
#include "../Header_Files/symbol_table.h"

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

int main() {
    struct dirent *de;
    DIR *dr = opendir(".");
    char input_filename[100];
    char output_filename[100];

    char filename[100];

    /* Reding and working on all .as files  */
    while ((de = readdir(dr)) != NULL) {
        if (strstr(de->d_name, ".as")) {
            printf("Processing file: %s\n", de->d_name);
            strncpy(filename, de->d_name, strlen(de->d_name) - 3);
            filename[strlen(de->d_name) - 3] = '\0';

            strcpy(input_filename, filename);
            strcat(input_filename, ".as");

            strcpy(output_filename, filename);
            strcat(output_filename, ".am");

            preprocess_macros(input_filename, output_filename);
            if (error_flag == 0) {
                printf("Finished macro expansion successfully. Starting first an second pass...\n");
                first_pass(output_filename);
                second_pass(output_filename);
            }
            if(error_flag == 0) {
                printf("Scanned first and second pass successfully. Creating output files...\n");
                create_output_files(filename);
            } else {
                printf("\033[1;31mErrors detected: \033[0m No output files created!\n");
            }
            reset_global_data();
        }
    }

    closedir(dr);
    return 0;
}
