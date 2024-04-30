
#ifndef ASSEMBLER_CODINGTOCODEIMAGEFIRSTPASS_H
#define ASSEMBLER_CODINGTOCODEIMAGEFIRSTPASS_H

#include "SettingLexTree.h"


#define BINARY_OPCODE_LENGTH 4




/*main function to code for Code Image, assuming lexTree.type = order */
void codeToCodeImage(lexTree*);

/*coding first word*/
void codeFirstWordInCodeImage(lexTree *);

/*return string that represent the binary opCode*/
char *getBinaryOpCode(commandOpcode);



#endif
