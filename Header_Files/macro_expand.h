#ifndef MACRO_EXPAND_H
#define MACRO_EXPAND_H

#define MAX_LINE_LENGTH 100
#define MAX_MACRO_COUNT 50
#define MAX_MACRO_LENGTH 1000
#define NULL ((void *)0)
typedef struct {
    char name[MAX_LINE_LENGTH];
    char content[MAX_MACRO_LENGTH];
} Macro;

extern Macro macros[MAX_MACRO_COUNT];
extern int macro_count;

void report_macro_error(const char *input_filename, int line_number, const char *message);
void trim_trailing_spaces(char *line);
void get_leading_whitespace(const char *line, char *out);
int is_macro_start(const char *line, char *name_out);
int is_macro_end(const char *line);
int find_macro(const char *name);
void store_macro(const char *input_filename, const char *name, const char *content, int line_number);
void preprocess_macros(const char *input_filename, const char *output_filename);
int is_valid_macro_name(const char *name);

#endif /* MACRO_EXPAND_H */
