#include <stdlib.h>
#include <string.h>

#include "firstPassCoding.h"
#include "SettingLexTree.h"
#include "baseTransitionUtiles.h"
#include "lexTreeValidation.h"
#include "memoryImage.h"
#include "parser.h"

void codeOrderToCodeImage(lexTree* thisLexTree, labelOrDefinitionList* openingLabelNDefinitionList) {
    char * firstMemoryWord;
    char *a;
    if(thisLexTree->error != valid)
        return;
    firstMemoryWord = getFirstWordInOrderImage(thisLexTree, openingLabelNDefinitionList);
    free(firstMemoryWord);
    if(determineAddressMethod(thisLexTree->content.orderContent.destinationOperand, openingLabelNDefinitionList) == immediateAddressing) {
        a = getImmediateAddressBinaryWord(&thisLexTree->content.orderContent.destinationOperand[SECOND_CELL_INDEX],
                                                openingLabelNDefinitionList);
        printf("%s\n", a);
        free(a);
    }
}

char* getImmediateAddressBinaryWord(const char* operand, labelOrDefinitionList* openingLabelNDefinitionList){
    char* word = (char*) malloc(IMAGE_WORD_IN_MEMORY_LENGTH * sizeof (char));
    char* ARE = "00";
    char* binaryOperand;
    char *endPointer;
    const char * rawOperand;
    if(is12BitsLegalNumberAsIs(operand))
        rawOperand = operand;
    else
        rawOperand = getDefinitionValueFromList(openingLabelNDefinitionList, operand);
    binaryOperand = intToBinaryString((int)strtol(rawOperand, &endPointer, DECIMAL), MEMORY_WORD_LENGTH_IN_CODE_IMAGE);
    strcpy(word, binaryOperand);
    free(binaryOperand);
    strcat(word, ARE);
    return word;
}

char* getFirstWordInOrderImage(lexTree* thisLexTree, labelOrDefinitionList* openingLabelNDefinitionList){
    OrderSentence thisOrderSentence = thisLexTree->content.orderContent;
    char * binaryOpCode, *binarySourcesAddressingMethod, *binaryDestinationAddressingMethod,
    * ARE, * lastFourBits;
    char *memoryWord = (char*)malloc(IMAGE_WORD_IN_MEMORY_LENGTH * sizeof (char));
    addressMethod sourceOperandAddressMethod;
    addressMethod destinationOperandAddressMethod;
    ARE = "00"; /* First order word always 00*/
    lastFourBits = "0000"; /* First order word always 0000 (unused)*/
    binaryOpCode = getBinaryOpCode(thisOrderSentence.opcode);
    sourceOperandAddressMethod = determineAddressMethod(
            thisOrderSentence.sourceOperand, openingLabelNDefinitionList);
    destinationOperandAddressMethod = determineAddressMethod(
            thisOrderSentence.destinationOperand, openingLabelNDefinitionList);
    if(sourceOperandAddressMethod == emptyOperand)
        sourceOperandAddressMethod = ZEROISE_ADDRESS_METHOD;
    if(destinationOperandAddressMethod == emptyOperand)
        destinationOperandAddressMethod = ZEROISE_ADDRESS_METHOD;
    binarySourcesAddressingMethod = getBinaryAddressingMethod(
            sourceOperandAddressMethod);
    binaryDestinationAddressingMethod = getBinaryAddressingMethod(
            destinationOperandAddressMethod);

    strcpy(memoryWord, lastFourBits);
    strcat(memoryWord, binaryOpCode);
    free(binaryOpCode);
    strcat(memoryWord, binarySourcesAddressingMethod);
    free(binarySourcesAddressingMethod);
    strcat(memoryWord, binaryDestinationAddressingMethod);
    free(binaryDestinationAddressingMethod);
    strcat(memoryWord,ARE);
    printf("%s \n",memoryWord);
    return memoryWord;
}

char* getBinaryAddressingMethod(addressMethod thisMethod){
    return intToBinaryString(thisMethod, BINARY_ADDRESSING_METHOD_LENGTH);
}

char* getBinaryOpCode(commandOpcode thisCommandOpCode){
    return intToBinaryString(thisCommandOpCode, BINARY_OPCODE_LENGTH);
}






