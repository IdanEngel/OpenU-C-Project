/* output.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Header_Files/output.h"
#include "../Header_Files/utils.h"
#include "../Header_Files/globals.h"

/* Base64 table for MMN14 */
const char *base64_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char *to_base64(unsigned short word) {
    char *res = (char *) malloc(3);
    res[0] = base64_table[(word >> 6) & 0x3F];
    res[1] = base64_table[word & 0x3F];
    res[2] = '\0';
    return res;
}

int write_object_file(const char *file_name, unsigned short *code, int total_count, int IC, int DC) {
    int i;
    FILE *fp;
    char *out_name = add_extension(file_name, ".ob");

    fp = fopen(out_name, "w");
    if (!fp) return 0;

    fprintf(fp, "%d %d\n", IC, DC);
    for (i = 0; i < total_count; i++) {
        char *b64 = to_base64(code[i]);
        fprintf(fp, "%s\n", b64);
        free(b64);
    }

    fclose(fp);
    free(out_name);
    return 1;
}

int write_entry_file(const char *file_name, void *entries_raw, int count) {
    FILE *fp;
    char *out_name;
    EntryLabel *entries = (EntryLabel *) entries_raw;

    if (count == 0) return 1; /* No file needed */

    out_name = add_extension(file_name, ".ent");
    fp = fopen(out_name, "w");
    if (!fp) return 0;

    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s %04d\n", entries[i].name, entries[i].address);
    }

    fclose(fp);
    free(out_name);
    return 1;
}

int write_extern_file(const char *file_name, void *externs_raw, int count) {
    FILE *fp;
    char *out_name;
    ExternUse *externs = (ExternUse *) externs_raw;

    if (count == 0) return 1;

    out_name = add_extension(file_name, ".ext");
    fp = fopen(out_name, "w");
    if (!fp) return 0;

    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s %04d\n", externs[i].name, externs[i].address);
    }

    fclose(fp);
    free(out_name);
    return 1;
}
