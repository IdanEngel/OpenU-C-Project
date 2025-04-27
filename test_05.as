.extern WRONG
.entry NOT_EXISTS

MAIN:   add r8, r31
        mov K, #5
        mcro BADMACRO
            sub r2, BAD_LABEL
        mcroend
        BADMACRO
        bne NOT_EXISTS
        jmp MISSING
K:      .data 5
