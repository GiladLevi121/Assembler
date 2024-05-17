; file ps.as
.entry LIST
h:
g: 100
.entry U
.define sz = 2
MAIN:	mov r3,kkkkk[sz]		
LOOP: 	jmp MAIN 
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

Removed .extern W, and used W.
Added .entry U, without declaring U in file.
Changed in Main line fixed addressing method to some undeclared label.
Added h label without any content