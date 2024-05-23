mcr m2
sub r1, r4
	cmp K, #sz
	bne W

endmcr

; file ps.as
.entry LIST 

mcr m1
.define len = 4
endmcr
.extern W 
.define sz = 2 
MAIN:	mov r3, LIST[sz]
LOOP: 	jmp W 
	prn #-5
	mov STR[5], STR[2] 
	m2
L1: 	inc L3 
.entry LOOP
	bne LOOP 
END: hlt
m1
STR: .string "abcdef" 
LIST: .data 6, -9, len 
K: .data 22 
.extern L3