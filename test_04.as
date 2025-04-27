.extern EXT_FUNC
.entry START

START:  lea STR, r1
        jsr EXT_FUNC
        cmp K, #-12
        bne &JUMP
        mcro SHORTCUT
            inc r3
            mov r1, r2
        mcroend
        SHORTCUT
JUMP: jmp &START
DONE:    stop
STR:     .string "World"
K:       .data 1, 2, 3
