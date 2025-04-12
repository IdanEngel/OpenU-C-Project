/* output.h */

#ifndef OUTPUT_H
#define OUTPUT_H

int write_object_file(const char *file_name, unsigned short *code, int total_count, int IC, int DC);
int write_entry_file(const char *file_name, void *entries, int count);
int write_extern_file(const char *file_name, void *externs, int count);

#endif
