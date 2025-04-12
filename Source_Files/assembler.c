/* assembler.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "assembler.h"
// #include "macro_processor.h"
// #include "first_pass.h"
// #include "second_pass.h"
// #include "output.h"
// #include "errors.h"
// #include "util.h"

void run_assembler(char *file_name) {
    char *input_file, *am_file;
    int error_found = 0;

    /* Step 1: Add .as extension to input file */
    input_file = add_extension(file_name, ".as");

    /* Step 2: Preprocess macros */
    if (!preprocess_file(input_file)) {
        report_error("Macro preprocessing failed", input_file, 0);
        free(input_file);
        return;
    }

    /* Step 3: Convert to .am file */
    am_file = add_extension(file_name, ".am");

    /* Step 4: First pass */
    if (first_pass(am_file, &error_found) == 0 && error_found) {
        report_error("First pass failed", am_file, 0);
        free(input_file);
        free(am_file);
        return;
    }

    /* Step 5: Second pass */
    if (second_pass(am_file, &error_found) == 0 && error_found) {
        report_error("Second pass failed", am_file, 0);
        free(input_file);
        free(am_file);
        return;
    }

    /* Step 6: Done! Cleanup */
    printf("File %s assembled successfully.\n", file_name);
    free(input_file);
    free(am_file);
}
//
// Created by ida24 on 10/04/2025.
//
