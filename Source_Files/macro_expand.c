#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "../Header_Files/macro_expand.h"
#include "../Header_Files/utils.h"

#define MAX_LINE_LENGTH 100
#define MAX_MACRO_COUNT 50
#define MAX_MACRO_LENGTH 1000

Macro macros[MAX_MACRO_COUNT];
int macro_count = 0;

void report_macro_error(const char *input_filename,  int line_number, const char *message) {
    printf("[File: %s, Line %d] Error: %s\n", input_filename, line_number, message);
}

void trim_trailing_spaces(char *line) {
    int len = strlen(line);
    while (len > 0 && isspace((unsigned char)line[len - 1])) {
        line[--len] = '\0';
    }
}

void get_leading_whitespace(const char *line, char *out) {
    int i = 0;
    while (line[i] == ' ' || line[i] == '\t') {
        out[i] = line[i];
        i++;
    }
    out[i] = '\0';
}

int is_macro_start(const char *line, char *name_out) {
    if (strncmp(line, "mcro", 4) == 0 && isspace(line[4])) {
        const char *p = line + 4;
        while (isspace(*p)) p++;
        if (*p == '\0') return -1;
        strncpy(name_out, p, MAX_LINE_LENGTH);
        return 1;
    }
    return 0;
}

int is_macro_end(const char *line) {
    return strncmp(line, "mcroend", 7) == 0;
}

int find_macro(const char *name) {
    int i;
    for (i = 0; i < macro_count; i++) {
        if (strcmp(macros[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

void store_macro(const char *input_filename, const char *name, const char *content, int line_number) {
    if (!isalpha(name[0])) {
        report_macro_error(input_filename, line_number, "Macro name must start with a letter.");
        return;
    }
    if (find_macro(name) != -1) {
        report_macro_error(input_filename, line_number, "Macro redefinition is not allowed.");
        return;
    }
    if (macro_count < MAX_MACRO_COUNT) {
        strncpy(macros[macro_count].name, name, MAX_LINE_LENGTH);
        strncpy(macros[macro_count].content, content, MAX_MACRO_LENGTH);
        macro_count++;
    }
}

void preprocess_macros(const char *input_filename, const char *output_filename) {
    FILE *input = fopen(input_filename, "r");
    FILE *output = fopen(output_filename, "w");
    char line[MAX_LINE_LENGTH];
    char macro_name[MAX_LINE_LENGTH];
    char macro_buffer[MAX_MACRO_LENGTH];
    int in_macro = 0;
    int macro_index;
    int line_number = 0;

    char original_line[MAX_LINE_LENGTH];
    char trimmed_line[MAX_LINE_LENGTH];
    char indent_string[MAX_LINE_LENGTH];
    int start_result;

    if (!input || !output) {
        printf("Error opening files.\n");
        return;
    }

    macro_buffer[0] = '\0';
    macro_name[0] = '\0';

    while (fgets(line, sizeof(line), input)) {
        line_number++;

        strncpy(original_line, line, MAX_LINE_LENGTH);
        line[strcspn(line, "\r\n")] = '\0';

        strncpy(trimmed_line, line, MAX_LINE_LENGTH);
        trim_leading_spaces(trimmed_line);
        trim_trailing_spaces(trimmed_line);

        get_leading_whitespace(original_line, indent_string);

        start_result = is_macro_start(trimmed_line, macro_name);
        if (start_result == -1) {
            report_macro_error(input_filename, line_number, "Macro defined without a name.");
            continue;
        } else if (start_result == 1) {
            if (in_macro) {
                report_macro_error(input_filename, line_number, "Nested macro definitions are not allowed.");
                continue;
            }
            in_macro = 1;
            macro_buffer[0] = '\0';
            continue;
        }

        if (is_macro_end(trimmed_line)) {
            if (!in_macro) {
                report_macro_error(input_filename, line_number, "mcroend found without preceding mcro.");
                continue;
            }
            store_macro(input_filename, macro_name, macro_buffer, line_number);
            in_macro = 0;
            continue;
        }

        if (in_macro) {
            strncat(macro_buffer, original_line, MAX_MACRO_LENGTH - strlen(macro_buffer) - 2);
            strncat(macro_buffer, "\n", MAX_MACRO_LENGTH - strlen(macro_buffer) - 1);
            continue;
        }

        macro_index = find_macro(trimmed_line);
        if (macro_index != -1) {
            const char *macro_content = macros[macro_index].content;
            char content_copy[MAX_MACRO_LENGTH];
            char *macro_line;
            strncpy(content_copy, macro_content, MAX_MACRO_LENGTH);
            macro_line = strtok(content_copy, "\n");
            while (macro_line != NULL) {
                fprintf(output, "%s\n", macro_line);
                macro_line = strtok(NULL, "\n");
            }
        } else if (strncmp(trimmed_line, ".", 1) != 0 && strncmp(trimmed_line, ";", 1) != 0 && strlen(trimmed_line) > 0) {
            if (strchr(trimmed_line, ' ') == NULL) {
                report_macro_error(input_filename, line_number, "Macro call to undefined macro.");
            }
            fprintf(output, "%s", original_line);
        } else {
            fprintf(output, "%s", original_line);
        }
    }

    if (in_macro) {
        report_macro_error(input_filename, line_number, "File ended before mcroend.");
    }

    fclose(input);
    fclose(output);
}
