/* parser.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../Header_Files/parser.h"
#include "../Header_Files/utils.h"

static char *strdup_safe(const char *str) {
    char *copy = (char *) safe_malloc(strlen(str) + 1);
    strcpy(copy, str);
    return copy;
}

Line parse_line(char *raw_line, int line_num) {
    Line result;
    char *token, *ptr;
    char line_copy[MAX_LINE_LENGTH];
    int index = 0;

    /* Init struct */
    memset(&result, 0, sizeof(Line));
    result.args[0] = NULL;
    result.args[1] = NULL;

    strncpy(line_copy, raw_line, MAX_LINE_LENGTH);
    line_copy[MAX_LINE_LENGTH - 1] = '\0';

    /* Tokenize */
    token = strtok(line_copy, " \t\n");
    if (!token) return result;  /* Empty or comment */

    /* Check for label */
    if (strchr(token, ':')) {
        ptr = strchr(token, ':');
        *ptr = '\0';
        result.label = strdup_safe(token);
        token = strtok(NULL, " \t\n");
        if (!token) {
            result.error = 1;
            strcpy(result.error_msg, "Label with no command or instruction.");
            return result;
        }
    }

    /* Opcode / directive */
    result.opcode = strdup_safe(token);

    /* Classify */
    if (strcmp(token, ".data") == 0 || strcmp(token, ".string") == 0) {
        result.is_data = 1;
    } else if (strcmp(token, ".extern") == 0) {
        result.is_extern = 1;
    } else if (strcmp(token, ".entry") == 0) {
        result.is_entry = 1;
    } else {
        result.is_command = 1;
    }

    /* Parse arguments */
    while ((token = strtok(NULL, ", \t\n")) != NULL) {
        if (index >= 2) {
            result.error = 1;
            strcpy(result.error_msg, "Too many arguments.");
            return result;
        }
        result.args[index++] = strdup_safe(token);
    }
    result.arg_count = index;

    return result;
}
