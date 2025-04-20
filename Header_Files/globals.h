/* globals.h */

#ifndef GLOBALS_H
#define GLOBALS_H
#define NULL ((void *)0)
/* Constants */
#define MAX_LABEL_LENGTH 31
#define MAX_SYMBOLS 1000
#define MAX_LINE_LENGTH 81
#define MAX_ARG_LEN 32

/* IC initialization value as per MMN14 */
#define IC_INIT 100

/* Data type for tracking .entry labels */
typedef struct {
    char name[MAX_LABEL_LENGTH + 1]; /* Label name */
    int address;                     /* Final address */
} EntryLabel;

/* Data type for tracking usage of .extern labels */
typedef struct {
    char name[MAX_LABEL_LENGTH + 1]; /* Label name */
    int address;                     /* Address where it's used */
} ExternUse;

#endif /* GLOBALS_H */
