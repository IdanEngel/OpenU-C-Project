#include <stdio.h>
#include "../Header_Files/assembler.h"

int main(int argc, char *argv[]) {
    int i;

    if (argc < 2) {
        printf("Usage: %s <file1> [file2] ...\n", argv[0]);
        return 1;
    }

    for (i = 1; i < argc; i++) {
        run_assembler(argv[i]);
    }

    return 0;
}
