#include <ctype.h>
#include <string.h>
#include "../Header_Files/utils.h"

/* Trim leading spaces */
void trim_leading_spaces(char *line) {
    int i = 0, j = 0;
    while (isspace((unsigned char)line[i])) i++;
    while (line[i]) line[j++] = line[i++];
    line[j] = '\0';
}
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
