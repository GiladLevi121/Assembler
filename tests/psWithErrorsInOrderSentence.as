; file ps.as
.entry LIST 
.extern W 
.define sz = 2 
MAIN:	mov r3, #-1
LOOP: 	jmp ,STR[2] 
	jmp W, STR[2] 
	jmp 
	prn 	
	mov , 
	sub r1, #2
	cmp , #sz
	cmp , 
	bne STR[2] 
L1: 	inc #2 
.entry LOOP
	bne #W
END: hlt #4
.define len = 4
STR: .string "abcdef" 
LIST: .data 6, -9, len 
K: .data 22 
.extern L3