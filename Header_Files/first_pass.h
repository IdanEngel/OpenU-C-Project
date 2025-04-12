/* first_pass.h */

#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include "globals.h"

/**
 * @file first_pass.h
 * @brief Handles the first pass over the assembly source code.
 */

/**
 * @brief Executes the first pass on a given .am file.
 *
 * @param am_file The file name of the preprocessed assembly file (.am)
 * @param error_flag Pointer to an int, set to 1 if any error occurs
 * @return 1 on success, 0 on failure
 */
int first_pass(const char *am_file, int *error_flag);

#endif /* FIRST_PASS_H */
