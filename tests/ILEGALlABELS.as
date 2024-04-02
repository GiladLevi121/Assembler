.define sz = 2
longLabelAbove31Numbers2222222222222222MAIN: mov r3, LIST[sz]
LABEL WITH UNLEGAL CHAR: jmp L1
prn #-5
mov STR[5], STR[2]
sub r1, r4
cmp r3, #sz
bne END
anotherLabelWithElegalChar;: inc K
bne LOOP
labelWithWiteSpaceAtEnd : hlt
.define len = 4
STR: .string "abcdef"
LIST: data 6, -9, len
K: .data 22