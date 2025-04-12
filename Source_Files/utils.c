/* util.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Header_Files/utils.h"

void *safe_malloc(size_t size) {
    void *ptr = malloc(size);
    if (!ptr) {
        printf("Memory allocation failed. Exiting.\n");
        exit(1);
    }
    return ptr;
}

char *add_extension(const char *file_name, const char *new_ext) {
    char *dot_pos;
    size_t len;
    char *new_name;

    /* Find dot in the original file name */
    dot_pos = strchr(file_name, '.');
    len = dot_pos ? (size_t)(dot_pos - file_name) : strlen(file_name);

    /* Allocate new string: base name + new extension + null terminator */
    new_name = (char *) safe_malloc(len + strlen(new_ext) + 1);

    strncpy(new_name, file_name, len);
    new_name[len] = '\0';
    strcat(new_name, new_ext);

    return new_name;
}
