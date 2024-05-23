

#ifndef ASSEMBLER_SECONDPASSCODDING_H
#define ASSEMBLER_SECONDPASSCODDING_H

#include "lexTree.h"
#include "label.h"
#include "entryExternDeclaration.h"
#include "lexTreeValidation.h"
#include "memoryImage.h"

/* Check if need coding for second pass, and codes if needed*/
void secondPassOrderCodingInToMemoryIfNeeded(lexTree *, labelOrDefinitionList *, entryExternList *, memoryImage*);

/* Check if need second pass extra coding*/
boolean isMissingCodingFromFirstPass(addressMethod destAddressMethod, addressMethod sourceAddressMethod);

/* Checks PC increment for this lexTree*/
int additionalWordsIncrementToPC(addressMethod, addressMethod);

/* Sets words from each operand*/
void setWordsAmountToCodeFromEachOperand(int* , int*, int*, addressMethod, addressMethod);

/* finds where the space holder, extract recoding value */
void reCodingToMemoryImageInsteadOfSpaceHolders(const char *sourceOperand, const char *destinationOperand, addressMethod sourceAddressMethod, addressMethod destAddressMethod,
                                                entryExternList *entryExternLabelList, memoryImage *fileMemoryImage, labelOrDefinitionList *openingLabelNDefinitionList);

/* Sets amount of words to code from operand, dont take-care case of both operands are
 * direct register method*/
void additionalWordsToCodeFromThisOperand(int*, addressMethod);

void resetSecondPassInnerIndex(int *innerPcIndex, addressMethod thisAddressMethod);

void reCodingThisSpacesHolder(const char *, addressMethod, entryExternList *, labelOrDefinitionList *,
                              memoryImage *, int *);


/* Return binary representation*/
char* getSecondPassBinaryWord(const char* , entryExternList *, labelOrDefinitionList*, int );

/* Recoding the new word to code image to + offset index*/
void reCodeThisWordToMemory(char*, memoryImage*, const int*);
#endif
