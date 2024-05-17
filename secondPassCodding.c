
#include <string.h>

#include "secondPassCodding.h"
#include "firstPassCoding.h"
#include "baseTransitionUtiles.h"



void secondPassOrderCodingInToMemoryIfNeeded(lexTree *thisLexTree,
                                             labelOrDefinitionList *openingLabelNDefinitionList,
                                             entryExternList *entryExternLabelList,
                                             memoryImage* fileMemoryImage){
    const char *sourceOperand = thisLexTree->content.orderContent.sourceOperand;
    const char *destinationOperand = thisLexTree->content.orderContent.destinationOperand;

    addressMethod destAddressMethod = determineAddressMethod(
            destinationOperand, openingLabelNDefinitionList);
    addressMethod sourceAddressMethod = determineAddressMethod(
            sourceOperand, openingLabelNDefinitionList);

    if (isMissingCodingFromFirstPass(destAddressMethod, sourceAddressMethod)){
        reCodingToMemoryImageInsteadOfSpaceHolders(sourceOperand, destinationOperand, sourceAddressMethod,
                                                   destAddressMethod, entryExternLabelList, fileMemoryImage,
                                                   openingLabelNDefinitionList);
    }else
        fileMemoryImage->secondPassCodeImageCounter +=/* ONE_WORD_TO_CODE + /*First coded word*/
                additionalWordsIncrementToPC(destAddressMethod, sourceAddressMethod);
}

void reCodingToMemoryImageInsteadOfSpaceHolders(const char* sourceOperand, const char* destinationOperand,
                                                addressMethod sourceAddressMethod, addressMethod destAddressMethod,
                                                entryExternList* entryExternLabelList, memoryImage *fileMemoryImage,
                                                labelOrDefinitionList* openingLabelNDefinitionList){
    int firstWordBuffer = ONE_WORD_TO_CODE, wordsFromSource = ZEROISE_COUNTER, wordsFromDestination = ZEROISE_COUNTER;
    if(sourceAddressMethod == directAddressing || sourceAddressMethod == fixedIndexAddressing){
        fileMemoryImage->secondPassCodeImageCounter += firstWordBuffer;
        reCodingThisSpacesHolder(sourceOperand, sourceAddressMethod, entryExternLabelList,
                                 openingLabelNDefinitionList, fileMemoryImage, &wordsFromSource);
        fileMemoryImage->secondPassCodeImageCounter += wordsFromSource;
    }else {
        resetSecondPassInnerIndex(&wordsFromSource, sourceAddressMethod);
        fileMemoryImage->secondPassCodeImageCounter += wordsFromSource + firstWordBuffer;
    }
    if(destAddressMethod == directAddressing || destAddressMethod == fixedIndexAddressing){
        reCodingThisSpacesHolder(destinationOperand, destAddressMethod, entryExternLabelList,
                                 openingLabelNDefinitionList, fileMemoryImage, &wordsFromDestination);
        fileMemoryImage->secondPassCodeImageCounter += wordsFromDestination;
    }else{
        resetSecondPassInnerIndex(&wordsFromDestination, destAddressMethod);
        fileMemoryImage->secondPassCodeImageCounter += wordsFromDestination;
    }
}


void reCodingThisSpacesHolder(const char* operand, addressMethod OperandAddressMethod,
                              entryExternList* entryExternLabelList,
                              labelOrDefinitionList * openingLabelNDefinitionList, memoryImage *fileMemoryImage,
                              int *offset){
    char* thisWord;
    if (OperandAddressMethod == directAddressing){
        thisWord = getSecondPassBinaryWord(operand, entryExternLabelList, openingLabelNDefinitionList);
        reCodeThisWordToMemory(thisWord, fileMemoryImage, offset);
        *offset = ONE_WORD_TO_CODE;
    }
    else /*(sourceAddressMethod == fixedIndexAddressing)*/{
        const char * indexOfSquareBrackets;
        char* arrayName;
        int arrayNameLength;
        indexOfSquareBrackets = strchr(operand, '[');
        arrayNameLength = (int)(indexOfSquareBrackets - operand);
        arrayName = (char*)malloc(sizeof (char) * (arrayNameLength + PADDING_CELL_LEN));
        strncpy(arrayName, operand, arrayNameLength + PADDING_CELL_LEN);
        arrayName[arrayNameLength] = END_OF_STRING;
        thisWord = getSecondPassBinaryWord(arrayName, entryExternLabelList, openingLabelNDefinitionList);
        free(arrayName);
        reCodeThisWordToMemory(thisWord, fileMemoryImage, offset);
        *offset = TWO_WORD_TO_CODE;
    }
}



int additionalWordsIncrementToPC(addressMethod destAddressMethod,
                                 addressMethod sourceAddressMethod){
    int wordsFromSource = ZEROISE_COUNTER, wordsFromDestination = ZEROISE_COUNTER,
            bothDirectRegister = ZEROISE_COUNTER;
    setWordsAmountToCodeFromEachOperand(&bothDirectRegister, &wordsFromSource,
                                        &wordsFromDestination, sourceAddressMethod, destAddressMethod);
    if(bothDirectRegister == ONE_WORD_TO_CODE)
        return TWO_WORD_TO_CODE; /* first word + this one... also for next case*/
    else
        return (wordsFromSource + wordsFromDestination + ONE_WORD_TO_CODE);

}


void setWordsAmountToCodeFromEachOperand(int* bothDirectRegister, int* wordsFromSource,
                                         int* wordsFromDestination, addressMethod sourceAddressMethod,
                                         addressMethod destAddressMethod){
    if(destAddressMethod == directRegisterAddressing && sourceAddressMethod == directRegisterAddressing){
        *bothDirectRegister = *bothDirectRegister + ONE_WORD_TO_CODE;
    }
    additionalWordsToCodeFromThisOperand(wordsFromSource, sourceAddressMethod);
    additionalWordsToCodeFromThisOperand(wordsFromDestination, destAddressMethod);
}

void additionalWordsToCodeFromThisOperand(int* wordsFromOperand, addressMethod thisAddressMethod){
    if(thisAddressMethod == immediateAddressing ||
    thisAddressMethod == directAddressing ||
    thisAddressMethod == directRegisterAddressing){
        *wordsFromOperand = *wordsFromOperand + ONE_WORD_TO_CODE;
    }else if(thisAddressMethod == fixedIndexAddressing){
        *wordsFromOperand = *wordsFromOperand + TWO_WORD_TO_CODE;
    }
}


void resetSecondPassInnerIndex(int *innerPcIndex, addressMethod thisAddressMethod){
    additionalWordsToCodeFromThisOperand(innerPcIndex, thisAddressMethod);
}



char* getSecondPassBinaryWord(const char* operandTitle, entryExternList *entryExternLabelList,
                              labelOrDefinitionList* openingLabelNDefinitionList){
    char* ARE, *addressValue;
    char* newWord = (char*) malloc((IMAGE_WORD_IN_MEMORY_LENGTH + PADDING_CELL_LEN) * sizeof(char));
    if(isTileAppearInEntryExternAsExternDeclarationList(operandTitle, entryExternLabelList)){
        ARE = "01";
        addressValue = (char*) malloc((MEMORY_WORD_LENGTH_IN_CODE_IMAGE + PADDING_CELL_LEN) * sizeof (char));
        strcpy(addressValue, "000000000000");
    }else{
        labelNode * foundedLabel;
        char *endPointer;
        int pcValue;
        ARE = "10";
        foundedLabel = getNodeIfAppearInLabelList(operandTitle, openingLabelNDefinitionList);
        pcValue = (int)strtol(foundedLabel->value.PC, &endPointer, DECIMAL);
        if (*endPointer != END_OF_STRING && *endPointer != END_OF_ROW){
            return NULL;
        }
        addressValue = intToBinaryString(pcValue, MEMORY_WORD_LENGTH_IN_CODE_IMAGE);
    }
    strcpy(newWord, addressValue);
    strcat(newWord, ARE);
    free(addressValue);
    return newWord;
}


void reCodeThisWordToMemory(char* word, memoryImage* fileMemoryImage, const int* offset){
    int wordIndex;
    wordIndex = (int)fileMemoryImage->secondPassCodeImageCounter + (*offset);
    if(fileMemoryImage->codeImage[wordIndex] == NULL){
        fileMemoryImage->codeImage[wordIndex] = word;
    }
}


boolean isMissingCodingFromFirstPass(addressMethod destAddressMethod, addressMethod sourceAddressMethod){
    if (destAddressMethod == fixedIndexAddressing || destAddressMethod == directAddressing ||
        sourceAddressMethod == fixedIndexAddressing || sourceAddressMethod == directAddressing)
        return true;
    return false;
}



