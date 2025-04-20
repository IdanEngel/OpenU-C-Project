#include <ctype.h>
#include "../Header_Files/utils.h"

/* Trim leading spaces */
void trim_leading_spaces(char *line) {
    int i = 0, j = 0;
    while (isspace((unsigned char)line[i])) i++;
    while (line[i]) line[j++] = line[i++];
    line[j] = '\0';
}
