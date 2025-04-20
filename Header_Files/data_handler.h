/* data_handler.h */

#ifndef DATA_HANDLER_H
#define DATA_HANDLER_H
#define NULL ((void *)0)
#include "parser.h"

/**
 * @brief Handles a .data or .string directive by appending to the data[] array.
 *
 * @param line Parsed Line struct from parser
 * @param data Data array to write into
 * @param DC Pointer to data counter (updated)
 * @return 1 on success, 0 on error
 */
int handle_data_instruction(Line line, unsigned short *data, int *DC);

#endif /* DATA_HANDLER_H */
