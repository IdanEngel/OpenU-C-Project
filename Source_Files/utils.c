#include <ctype.h>
#include <string.h>
#include "../Header_Files/utils.h"


/*
 * Remove leading whitespace (spaces and tabs) from a line.
 * Modifies the input string directly by shifting characters.
 */
void trim_leading_spaces(char *line) {
    int i = 0, j = 0;
    while (isspace((unsigned char)line[i])) i++;
    while (line[i]) line[j++] = line[i++];
    line[j] = '\0';
}
/*
 * Remove all spaces and tabs from a string.
 * Shifts characters left to compact the string without whitespace.
 */
void trim_spaces(char *str) {
    char *end;

    while (isspace((unsigned char)*str)) str++;

    if (*str == 0) {
        str[0] = '\0';
        return;
    }

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';
}
