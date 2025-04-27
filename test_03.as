.extern EXT_LABEL
.entry MAIN

MAIN:   mov r2, EXT_LABEL
        add r3, COUNT
        mcro A_MACRO
            inc r2
            dec r3
        mcroend
        A_MACRO
        cmp r2, #5
        bne END
LOOP:   prn #66
        sub r3, r4
        lea STR, r5
        bne &LOOP
END:    stop
STR:    .string "Hello"
COUNT:  .data 6, -3, 15
