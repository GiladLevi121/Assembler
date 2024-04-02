STR: .string "abcd ef" 
STR: .string   "abcd ef"    

.string
.string         
STR: .string "abcd ef" d
STR: .string s "abcd ef" 
STR: .string "abcd ef"s
STR: .string f"abcd ef" 
 
LIST:           .data      6, -9  3, len 
LIST: .data    6 ,   -9  3, len     

MAIN:	mov   r3   ,   LIST[sz]   32
MAIN:	mov   r3   ,   LIST[sz] ,  32
mov   r3  dvdv ,   LIST[sz]   32
LOOP: 	jmp    W    5, 
LOOP: 	jmp    W   , 5
	prn #-5   
prn #-5 , dfg
prn #-5, 4
	mov    STR[5]  ,   STR[2]    
	sub r1  ,   r4   
	cmp K, #sz
	bne W
L1: 	inc L3 
.entry LOOP
	bne LOOP 
END: hlt      
.define len = 2
STR: .string "abcdef" 
LIST: .data 6, -9, len 
K: .data 22 
.extern L3


; file ps.as
.entry LIST 
.extern W 
.define    sz = 2
