
#ifndef ASSEMBLER_FIRSTPASSCODING_H
#define ASSEMBLER_FIRSTPASSCODING_H

#include "SettingLexTree.h"
#include "memoryImage.h"
#include "lexTreeValidation.h"

#define ZEROISE_ADDRESS_METHOD 0
#define BINARY_ADDRESSING_METHOD_LENGTH 2
#define BINARY_OPCODE_LENGTH 4


/*------------------------------direction functions------------------------------*/
char** getBinaryRepresentationOfDirection(lexTree *, int *,
                                        labelOrDefinitionList* );

char **getBinaryRepresentationOfData(lexTree *, int *,
                                     labelOrDefinitionList* );

char **getBinaryRepresentationOfString(lexTree *, int *);
/*------------------------------orders functions------------------------------*/

/* gets all relevant data, and returns **char of this instruction in binary.
 * Not-looking-good-decision-tree*/
char** getTheRestBinaryInstruction(char * firstMemoryWord, char **fixedIndexSource, char **fixedIndexDestination,
                                   char *oneWordSource, char *oneWordDestination, char *bothDirectRegisterContent,
                                   int wordsFromSource, int wordsFromDestination, int bothDirectRegister,
                                   int * overAllWords);

/* Return direct register word in case both operands are direct register method*/
char* getBothOperandDirectRegisterWord(const char*,const char*);

/* Return direct register word, enter true for source operand, false for destination*/
char* getDirectRegisterWord(const char*, boolean);

/* Return 2 words, one space holder and the second is the index in binary*/
char** getTwoWordsOfFixedIndexAddress(const char*, labelOrDefinitionList*);

/* Uses "getSpaceHolder"*/
char* getFixedAddressFirstWordSpaceHolder();

/* Return binary word of the second word in fixed address method*/
char* getBinaryIndexRepresentationOfFixedIndex(const char *operand, labelOrDefinitionList *openingLabelNDefinitionList);

/*main function to code for Code Image, assuming lexTree.type = order */
char** getBinaryRepresentationOfThisOrder(lexTree *thisLexTree, int* wordsOfMemoryInThisOrder,
                                        labelOrDefinitionList *openingLabelNDefinitionList);

/* Returns immediate address method binary representation*/
char* getImmediateAddressBinaryWord(const char*, labelOrDefinitionList*);

/* Uses "getSpaceHolder"*/
char *getDirectAddressSpaceHolder();

/* Allocating 14bits word, assigned null, and return*/
char* getSpaceHolder();

/*Returns first memory word of order sentence, in binary representation*/
char* getFirstWordInOrderImage(lexTree *thisLexTree, labelOrDefinitionList *openingLabelNDefinitionList);

/*return string that represent the binary opCode*/
char *getBinaryOpCode(commandOpcode);

/*return string that represent the binary addressing method*/
char* getBinaryAddressingMethod(addressMethod);

#endif
