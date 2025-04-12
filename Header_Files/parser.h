/* parser.h */

#ifndef PARSER_H
#define PARSER_H

#define MAX_LINE_LENGTH 81
#define MAX_ARG_LEN 32

typedef struct {
    char *label;          /* e.g., "LOOP" from "LOOP: mov ..." */
    char *opcode;         /* e.g., "mov" or ".data" */
    char *args[2];        /* Up to 2 arguments */
    int is_data;          /* 1 if .data or .string */
    int is_extern;        /* 1 if .extern */
    int is_entry;         /* 1 if .entry */
    int is_command;       /* 1 if opcode is real command (mov, cmp, ...) */
    int arg_count;        /* 0, 1, or 2 */
    int error;            /* 1 if parsing error occurred */
    char error_msg[100];  /* Explanation of the error */
} Line;

/**
 * @brief Parses an assembly line into label/opcode/args structure.
 *
 * @param raw_line The text of the line.
 * @param line_num Line number (for error messages).
 * @return A Line struct with parsed components and error info.
 */
Line parse_line(char *raw_line, int line_num);

#endif /* PARSER_H */
