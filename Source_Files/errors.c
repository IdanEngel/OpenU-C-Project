/* errors.c */

#include <stdio.h>
#include <stdlib.h>
#include "../Header_Files/errors.h"

void report_error(const char *msg, const char *file_name, int line_num) {
    if (file_name != NULL && line_num > 0) {
        printf("Error in %s at line %d: %s\n", file_name, line_num, msg);
    } else if (file_name != NULL) {
        printf("Error in %s: %s\n", file_name, msg);
    } else {
        printf("Error: %s\n", msg);
    }
}

void fatal_error(const char *msg) {
    printf("Fatal error: %s\n", msg);
    exit(1);
}
