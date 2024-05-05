#include <stdlib.h>
#include <string.h>

#include "firstPassCoding.h"
#include "SettingLexTree.h"
#include "baseTransitionUtiles.h"
#include "lexTreeValidation.h"
#include "memoryImage.h"

void codeOrderToCodeImage(lexTree* thisLexTree, labelOrDefinitionList* openingLabelNDefinitionList) {
    if(thisLexTree->error != valid)
        return;
    free(getFirstWordInOrderImage(thisLexTree, openingLabelNDefinitionList));
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






