; file ps.as


mcr M2
.define sz = 2 

endmcr
.entry LIST 
M2
mcr M1
prn #-5
	mov STR[5], STR[2] 
	sub r1, r4
endmcr
.extern W 

MAIN:	mov r3, LIST[sz]
LOOP: 	jmp W 
	prn #-5
	M1
	bne W
L1: 	inc L3 
.entry LOOP
	bne LOOP 
END: hlt
.define len = 4
STR: .string "abcdef" 
LIST: .data 6, -9, len 
K: .data 22 
.extern L3