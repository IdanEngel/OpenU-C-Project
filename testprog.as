.extern SUBR
MAIN:   add     r3, SUB
LOOP:   prn     #48
        lea     STR, r6
        mcro    a_mc
        cmp     K, #-6
        bne     &END
        mcroend
        inc     r6
        mov     r3, K
        sub     r1, r4
        bne     END
        a_mc
        dec     K
        jmp     &LOOP
.entry LOOP
END:    stop
STR:    .string "abcd"
LIST:   .data 6, -9
        .data -100
K:      .data 31


