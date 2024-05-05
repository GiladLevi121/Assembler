
#ifndef ASSEMBLER_FIRSTPASSCODING_H
#define ASSEMBLER_FIRSTPASSCODING_H

#include "SettingLexTree.h"
#include "memoryImage.h"
#include "lexTreeValidation.h"

#define ZEROISE_ADDRESS_METHOD 0
#define BINARY_ADDRESSING_METHOD_LENGTH 2
#define BINARY_OPCODE_LENGTH 4




/*main function to code for Code Image, assuming lexTree.type = order */
void codeOrderToCodeImage(lexTree*, labelOrDefinitionList*);

/*coding first word*/
char* getFirstWordInOrderImage(lexTree *thisLexTree, labelOrDefinitionList *openingLabelNDefinitionList);

/*return string that represent the binary opCode*/
char *getBinaryOpCode(commandOpcode);

/*return string that represent the binary addressing method*/
char* getBinaryAddressingMethod(addressMethod);

#endif
