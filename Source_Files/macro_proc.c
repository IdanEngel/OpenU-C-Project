//
///* macro_processor.c */
//
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include "../Header_Files/macro_processor.h"
//#include "../Header_Files/utils.h"
//
//#define MAX_LINE_LEN 100
//#define MACRO_NAME_LEN 32
//
//typedef struct Macro {
//    char name[MACRO_NAME_LEN];
//    char *content;
//    struct Macro *next;
//} Macro;
//
//static Macro *macro_list = NULL;
//
///* Internal helpers */
//static Macro *find_macro(const char *name);
//static void add_macro(const char *name, const char *content);
//static void free_macros();
//
//int preprocess_file(const char *file_name) {
//    char line[MAX_LINE_LEN];
//    char *macro_name, *macro_content;
//    int in_macro = 0;
//    FILE *fp_in, *fp_out;
//    char *out_file;
//    Macro *found;
//
//    fp_in = fopen(file_name, "r");
//    if (!fp_in) return 0;
//
//    out_file = add_extension(file_name, ".am");
//    fp_out = fopen(out_file, "w");
//    if (!fp_out) {
//        fclose(fp_in);
//        return 0;
//    }
//
//    while (fgets(line, MAX_LINE_LEN, fp_in)) {
//        if (!in_macro) {
//            if (strncmp(line, "mcro ", 5) == 0) {
//                in_macro = 1;
//                macro_name = strtok(line + 5, " \n");
//                macro_content = malloc(1);
//                macro_content[0] = '\0';
//                continue;
//            }
//
//            found = find_macro(line);
//            if (found) {
//                fprintf(fp_out, "%s", found->content);
//            } else {
//                fprintf(fp_out, "%s", line);
//            }
//        } else {
//            if (strncmp(line, "endmcro", 7) == 0) {
//                add_macro(macro_name, macro_content);
//                free(macro_content);
//                in_macro = 0;
//            } else {
//                macro_content = realloc(macro_content, strlen(macro_content) + strlen(line) + 1);
//                strcat(macro_content, line);
//            }
//        }
//    }
//
//    fclose(fp_in);
//    fclose(fp_out);
//    free(out_file);
//    free_macros();
//    return 1;
//}
//
///* Lookup macro by name */
//static Macro *find_macro(const char *line) {
//    Macro *curr = macro_list;
//    while (curr) {
//        if (strncmp(line, curr->name, strlen(curr->name)) == 0 &&
//            (line[strlen(curr->name)] == ' ' || line[strlen(curr->name)] == '\n')) {
//            return curr;
//        }
//        curr = curr->next;
//    }
//    return NULL;
//}
//
///* Add a new macro to the list */
//static void add_macro(const char *name, const char *content) {
//    Macro *new_macro = malloc(sizeof(Macro));
//    strncpy(new_macro->name, name, MACRO_NAME_LEN);
//    new_macro->content = malloc(strlen(content) + 1);
//    strcpy(new_macro->content, content);
//    new_macro->next = macro_list;
//    macro_list = new_macro;
//}
//
///* Free all macros after use */
//static void free_macros() {
//    Macro *curr = macro_list;
//    while (curr) {
//        Macro *temp = curr;
//        curr = curr->next;
//        free(temp->content);
//        free(temp);
//    }
//    macro_list = NULL;
//}
