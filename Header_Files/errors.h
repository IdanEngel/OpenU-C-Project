/* errors.h */

#ifndef ERRORS_H
#define ERRORS_H
#define NULL ((void *)0)
/**
 * @file errors.h
 * @brief Error reporting interface for the assembler.
 */

#include "globals.h"  /* For location struct */

/**
 * @brief Prints an error message with optional file and line info.
 *
 * @param msg The error message
 * @param file_name The name of the file where the error occurred
 * @param line_num The line number (use 0 if unknown)
 */
void report_error(const char *msg, const char *file_name, int line_num);

/**
 * @brief Prints an internal error (e.g., memory failure, logic bug).
 *
 * @param msg The message to show before aborting
 */
void fatal_error(const char *msg);

#endif /* ERRORS_H */
