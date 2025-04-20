///* data_handler.c */
//
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include "../Header_Files/data_handler.h"
//#include "../Header_Files/errors.h"
//#include "../Header_Files/utils.h"
//
//int handle_data_instruction(Line line, unsigned short *data, int *DC) {
//    int i;
//
//    if (line.is_data && strcmp(line.opcode, ".data") == 0) {
//        for (i = 0; i < line.arg_count; i++) {
//            int value = atoi(line.args[i]);
//            if (value < -2048 || value > 2047) {
//                return 0;
//            }
//            data[(*DC)++] = (unsigned short)value;
//        }
//        return 1;
//    }
//
//    if (line.is_data && strcmp(line.opcode, ".string") == 0) {
//        char *str = line.args[0];
//        int len;
//
//        if (!str || str[0] != '"' || str[strlen(str) - 1] != '"') {
//            return 0;
//        }
//
//        str++;
//        str[strlen(str) - 1] = '\0';
//        len = strlen(str);
//
//        for (i = 0; i < len; i++) {
//            data[(*DC)++] = (unsigned short)str[i];
//        }
//
//        data[(*DC)++] = 0;
//        return 1;
//    }
//
//    return 0;
//}
