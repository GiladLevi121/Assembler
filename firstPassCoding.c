#include <stdlib.h>
#include <string.h>

#include "firstPassCoding.h"
#include "SettingLexTree.h"
#include "baseTransitionUtiles.h"
#include "lexTreeValidation.h"
#include "memoryImage.h"
#include "parser.h"


/*------------------------------direction functions------------------------------*/
char** getBinaryRepresentationOfDirection(lexTree *thisLexTree, int *wordsOfMemoryInThisDirection,
                                        labelOrDefinitionList* openingLabelNDefinitionList){
    if(thisLexTree->content.directionSentence.type == dataDirection){
        return getBinaryRepresentationOfData(thisLexTree, wordsOfMemoryInThisDirection,
                                             openingLabelNDefinitionList);
    }else if(thisLexTree->content.directionSentence.type == stringDirection){
        return getBinaryRepresentationOfString(thisLexTree, wordsOfMemoryInThisDirection);
    }
    return NULL;
}


char **getBinaryRepresentationOfData(lexTree *thisLexTree, int *wordsOfMemoryInThisDirection,
                                     labelOrDefinitionList* openingLabelNDefinitionList){
    DirectionSentence thisDirection = thisLexTree->content.directionSentence;
    size_t argumentAmount = getArgumentAmountInDataContent(thisLexTree);
    char** wordsToCode;
    int i = ZEROISE_COUNTER, finalOperand;
    char* definitionValue, *endPointer;
    *wordsOfMemoryInThisDirection = (int)argumentAmount;
    wordsToCode = (char**) malloc(argumentAmount * sizeof (char*));

    for(; i < argumentAmount; i++){
        if(is14BitsLegalNumberAsIs(thisDirection.content.dataDirection[i]))
            finalOperand = (int)strtol(thisDirection.content.dataDirection[i], &endPointer, DECIMAL);
        else{
            definitionValue = getDefinitionValueFromListIgnoreWhiteSpaces(
                    openingLabelNDefinitionList, thisDirection.content.dataDirection[i]);
            finalOperand = (int)strtol(definitionValue, &endPointer, DECIMAL);
        }
        if (*endPointer != END_OF_STRING && *endPointer != END_OF_ROW)
            return NULL;
        wordsToCode[i] = intToBinaryString(finalOperand, IMAGE_WORD_IN_MEMORY_LENGTH);
    }
    return wordsToCode;
}

char **getBinaryRepresentationOfString(lexTree *thisLexTree,
                                       int *wordsOfMemoryInThisDirection){
    //printf("%s\n",thisLexTree->content.directionSentence.content.stringContent);
    DirectionSentence thisString = thisLexTree->content.directionSentence;
    int i = ZEROISE_COUNTER + ANOTHER_CELL;/* stringContent[0] = '"'*/
    /* <=> strlen(string) - len('"') - len('"') + len(endOfString) = len - 1 - 1 + 1*/
    size_t amountOfSinglePointers = strlen(thisString.content.stringContent) - ANOTHER_CELL;
    char** stringInBinary = (char**)malloc(amountOfSinglePointers * sizeof (char*));
    for(; thisString.content.stringContent[i] != '"'; i++){
        stringInBinary[i - ANOTHER_CELL] = charToBinaryString(thisString.content.stringContent[i]);
    }
    stringInBinary[i - ANOTHER_CELL] = (char*) malloc(IMAGE_WORD_IN_MEMORY_LENGTH * sizeof(char));
    strcpy(stringInBinary[i - ANOTHER_CELL], "00000000000000");
    *wordsOfMemoryInThisDirection = amountOfSinglePointers;
    return stringInBinary;
}


/*------------------------------orders functions------------------------------*/
char** getBinaryRepresentationOfThisOrder(lexTree* thisLexTree, int* wordsOfMemoryInThisOrder,
                                        labelOrDefinitionList* openingLabelNDefinitionList) {
    const char *sourceOperand = thisLexTree->content.orderContent.sourceOperand;
    const char * destinationOperand = thisLexTree->content.orderContent.destinationOperand;
    char *firstMemoryWord;
    char **thisWordsToCode, **fixedIndexSource = NULL, **fixedIndexDestination = NULL,
    *oneWordSource = NULL, *oneWordDestination = NULL, *bothDirectRegisterContent = NULL;
    int wordsFromSource = ZEROISE_COUNTER, wordsFromDestination = ZEROISE_COUNTER,
    bothDirectRegister = ZEROISE_COUNTER;
    addressMethod destAddressMethod = determineAddressMethod(
            destinationOperand, openingLabelNDefinitionList);
    addressMethod sourceAddressMethod = determineAddressMethod(
            sourceOperand, openingLabelNDefinitionList);

    if(thisLexTree->error != valid)
        return NULL;
    firstMemoryWord = getFirstWordInOrderImage(thisLexTree, openingLabelNDefinitionList);
    if(destAddressMethod == directRegisterAddressing && sourceAddressMethod == directRegisterAddressing){
        bothDirectRegister ++;
        bothDirectRegisterContent = getBothOperandDirectRegisterWord(sourceOperand, destinationOperand);
    }
    if(sourceAddressMethod == immediateAddressing){
        oneWordSource = getImmediateAddressBinaryWord(sourceOperand, openingLabelNDefinitionList);
        wordsFromSource++;
    }else if(sourceAddressMethod == directAddressing){
        oneWordSource = getDirectAddressSpaceHolder();
        wordsFromSource++;
    }else if (sourceAddressMethod == directRegisterAddressing){
        oneWordSource = getDirectRegisterWord(sourceOperand, true);
        wordsFromSource++;
    }else if(sourceAddressMethod == fixedIndexAddressing){
        fixedIndexSource = getTwoWordsOfFixedIndexAddress(sourceOperand, openingLabelNDefinitionList);
        wordsFromSource += TWO_WORD_TO_CODE;
    }
    if(destAddressMethod == immediateAddressing){
        oneWordDestination = getImmediateAddressBinaryWord(destinationOperand, openingLabelNDefinitionList);
        wordsFromDestination++;
    } else if(destAddressMethod == directAddressing){
        oneWordDestination = getDirectAddressSpaceHolder();
        wordsFromDestination++;
    }else if (sourceAddressMethod != directRegisterAddressing && destAddressMethod == directRegisterAddressing){
        oneWordDestination = getDirectRegisterWord(destinationOperand, false);
        wordsFromDestination++;
    }else if(destAddressMethod == fixedIndexAddressing){
        fixedIndexDestination = getTwoWordsOfFixedIndexAddress(destinationOperand, openingLabelNDefinitionList);
        wordsFromDestination += TWO_WORD_TO_CODE;
    }

    thisWordsToCode = getTheRestBinaryInstruction(firstMemoryWord, fixedIndexSource, fixedIndexDestination,
                                                  oneWordSource,
                                                  oneWordDestination, bothDirectRegisterContent, wordsFromSource,
                                                  wordsFromDestination, bothDirectRegister, wordsOfMemoryInThisOrder);
    return thisWordsToCode;
}

char** getTheRestBinaryInstruction(char * firstMemoryWord, char **fixedIndexSource, char **fixedIndexDestination,
                                   char *oneWordSource, char *oneWordDestination, char *bothDirectRegisterContent,
                                   int wordsFromSource, int wordsFromDestination, int bothDirectRegister,
                                   int * overAllWords){
    char** thoseMemoryWords;
    if(bothDirectRegister){
        thoseMemoryWords = (char**) malloc(TWO_WORD_TO_CODE * sizeof(char*));
        thoseMemoryWords[FIRST_WORD_INDEX] = firstMemoryWord;
        thoseMemoryWords[SECOND_WORD_INDEX] = bothDirectRegisterContent;
        *overAllWords = TWO_WORD_TO_CODE;
        return thoseMemoryWords;
    }
    *overAllWords = wordsFromDestination + wordsFromSource + FIRS_WORD_OCCUPIED;
    if(wordsFromSource == ZERO_TOKENS) /* empty source*/{
        if(wordsFromDestination == ZERO_TOKENS){
            thoseMemoryWords = (char**) malloc(ONE_WORD_TO_CODE * sizeof(char*));
            thoseMemoryWords[FIRST_WORD_INDEX] = firstMemoryWord;
            return thoseMemoryWords;
        }else if(wordsFromDestination == TWO_WORD_TO_CODE){
            thoseMemoryWords = (char**) malloc(THREE_WORD_TO_CODE * sizeof(char*));
            thoseMemoryWords[FIRST_WORD_INDEX] = firstMemoryWord;
            thoseMemoryWords[SECOND_WORD_INDEX] = fixedIndexDestination[FIRST_WORD_INDEX];
            thoseMemoryWords[THIRD_WORD_INDEX] = fixedIndexDestination[SECOND_WORD_INDEX];
            return thoseMemoryWords;
        }else /*wordsFromDestination == ONE_WORD_TO_CODE*/{
            thoseMemoryWords = (char**) malloc(TWO_WORD_TO_CODE * sizeof(char*));
            thoseMemoryWords[FIRST_WORD_INDEX] = firstMemoryWord;
            thoseMemoryWords[SECOND_WORD_INDEX] = oneWordDestination;
            return thoseMemoryWords;
        }
    }
    else if(wordsFromSource == ONE_WORD_TO_CODE) {
        if(wordsFromDestination == ZERO_TOKENS){
            thoseMemoryWords = (char**) malloc(*overAllWords * sizeof(char*));
            thoseMemoryWords[FIRST_WORD_INDEX] = firstMemoryWord;
            thoseMemoryWords[SECOND_WORD_INDEX] = oneWordSource;
            return thoseMemoryWords;
        }else if(wordsFromDestination == TWO_WORD_TO_CODE){
            thoseMemoryWords = (char**) malloc(*overAllWords * sizeof(char*));
            thoseMemoryWords[FIRST_WORD_INDEX] = firstMemoryWord;
            thoseMemoryWords[SECOND_WORD_INDEX] = oneWordSource;
            thoseMemoryWords[THIRD_WORD_INDEX] = fixedIndexDestination[FIRST_WORD_INDEX];
            thoseMemoryWords[FOURTH_WORD_INDEX] = fixedIndexDestination[SECOND_WORD_INDEX];
            return thoseMemoryWords;
        }else /*wordsFromDestination == ONE_WORD_TO_CODE*/{
            thoseMemoryWords = (char**) malloc(*overAllWords * sizeof(char*));
            thoseMemoryWords[FIRST_WORD_INDEX] = firstMemoryWord;
            thoseMemoryWords[SECOND_WORD_INDEX] = oneWordSource;
            thoseMemoryWords[THIRD_WORD_INDEX] = oneWordDestination;
            return thoseMemoryWords;
        }
    }else /*wordsFromSource == TWO_WORD_TO_CODE*/{
        if(wordsFromDestination == ZERO_TOKENS){
            thoseMemoryWords = (char**) malloc(*overAllWords * sizeof(char*));
            thoseMemoryWords[FIRST_WORD_INDEX] = firstMemoryWord;
            thoseMemoryWords[SECOND_WORD_INDEX] = fixedIndexSource[FIRST_WORD_INDEX];
            thoseMemoryWords[THIRD_WORD_INDEX] = fixedIndexSource[SECOND_WORD_INDEX];
            return thoseMemoryWords;
        }else if(wordsFromDestination == TWO_WORD_TO_CODE){
            thoseMemoryWords = (char**) malloc(*overAllWords * sizeof(char*));
            thoseMemoryWords[FIRST_WORD_INDEX] = firstMemoryWord;
            thoseMemoryWords[SECOND_WORD_INDEX] = fixedIndexSource[FIRST_WORD_INDEX];
            thoseMemoryWords[THIRD_WORD_INDEX] = fixedIndexSource[SECOND_WORD_INDEX];
            thoseMemoryWords[FOURTH_WORD_INDEX] = fixedIndexDestination[FIRST_WORD_INDEX];
            thoseMemoryWords[FIFTH_WORD_INDEX] = fixedIndexDestination[SECOND_WORD_INDEX];
            return thoseMemoryWords;
        }else /*wordsFromDestination == ONE_WORD_TO_CODE*/{
            thoseMemoryWords = (char**) malloc(*overAllWords * sizeof(char*));
            thoseMemoryWords[FIRST_WORD_INDEX] = firstMemoryWord;
            thoseMemoryWords[SECOND_WORD_INDEX] = fixedIndexSource[FIRST_WORD_INDEX];
            thoseMemoryWords[THIRD_WORD_INDEX] = fixedIndexSource[SECOND_WORD_INDEX];
            thoseMemoryWords[FOURTH_WORD_INDEX] = oneWordDestination;
            return thoseMemoryWords;
        }
    }
    printf("CRITICAL ERROR IN DECISION TREE");
    return NULL;
}

char* getBothOperandDirectRegisterWord(const char* sourceOperand,const char* destinationOperand){
    char *word = (char*)malloc(IMAGE_WORD_IN_MEMORY_LENGTH * sizeof (char));
    char *sourceInBinary, *destinationInBinary;
    int counter = ZEROISE_COUNTER;
    sourceInBinary = getDirectRegisterWord(sourceOperand , true);
    destinationInBinary = getDirectRegisterWord(destinationOperand, false);
    for(; counter < IMAGE_WORD_IN_MEMORY_LENGTH; counter++){
        if(sourceInBinary[counter] == '1' || destinationInBinary[counter] == '1')
            word[counter] = '1';
        else
            word[counter] = '0';
    }
    word[IMAGE_WORD_IN_MEMORY_LENGTH] = END_OF_STRING;
    free(sourceInBinary);
    free(destinationInBinary);
    return word;
}

char* getDirectRegisterWord(const char* operand, boolean sourceOrDestination){ /* Source = true*/
    char *word = (char*) malloc(IMAGE_WORD_IN_MEMORY_LENGTH * sizeof (char));
    char * ARE = "00", *endPointer;
    char *lastSixBits = "000000";
    char *threeNotUsedBits = "000";
    char *registerInBinary;
    int i = ZEROISE_COUNTER, regNumber;
    for(; i < REGISTERS_AMOUNT; i++){
        if (!strcmp(reservedAssemblyWords[REGISTERS_FIRST_INDEX_IN_KEY_WORDS + i], operand)){
            regNumber = (int)strtol(
                    reservedAssemblyWords[REGISTERS_FIRST_INDEX_IN_KEY_WORDS + i] + ANOTHER_CELL,/* skips 'r'*/
                    &endPointer, DECIMAL);
            if (*endPointer != END_OF_STRING && *endPointer != END_OF_ROW){
                free(word);
                return NULL;
            }
        }
    }
    registerInBinary = intToBinaryString(regNumber, REG_NAME_LENGTH);
    strcpy(word, lastSixBits);
    if(sourceOrDestination == true){
        strcat(word, registerInBinary);
        strcat(word, threeNotUsedBits);
    }else{
        strcat(word, threeNotUsedBits);
        strcat(word, registerInBinary);
    }
    free(registerInBinary);
    strcat(word, ARE);
    return word;
}

char** getTwoWordsOfFixedIndexAddress(
        const char* operand, labelOrDefinitionList* openingLabelNDefinitionList){
    char* firstWordSpaceHolder = getFixedAddressFirstWordSpaceHolder();
    char* binarySecondWord = getBinaryIndexOfFixedIndex(operand, openingLabelNDefinitionList);
    char** words = (char**)malloc(TWO_WORDS * sizeof (char*));
    words[FIRST_INDEX] = firstWordSpaceHolder;
    words[SECOND_CELL_INDEX] = binarySecondWord;
    return words;
}

char* getBinaryIndexOfFixedIndex(
        const char* operand, labelOrDefinitionList* openingLabelNDefinitionList){
    char *binaryWord = (char *)malloc(IMAGE_WORD_IN_MEMORY_LENGTH * sizeof (char));
    char *ARE = "00"; /* Like immediate addressing*/
    char* indexName, *definitionValue;
    char token[MAX_NUMBERS_IN_DATA_DECLARATION], *endPointer, *binaryOperand;
    int finalOperand;
    const char* indexOfSquareBrackets = strchr(operand, '['); /*gets the beginning of the inner index*/
    int arrayNameLength = (int)(indexOfSquareBrackets - operand);
    int lengthOfInnerIndex = (int)(strlen(operand) - arrayNameLength) - ANOTHER_CELL - LAST_CELL;/*- len('[') - len(']')*/
    indexName = (char*)malloc(sizeof (char) * (lengthOfInnerIndex));
    strncpy(token, indexOfSquareBrackets + LAST_CELL , lengthOfInnerIndex);
    indexName = trimLeadingNEndingWhitespace(token);
    /*indexName[lengthOfInnerIndex] = END_OF_STRING;*/
    if(is12BitsLegalNumberAsIs(indexName))
        finalOperand = (int)strtol(indexName, &endPointer, DECIMAL);
    else{
        definitionValue = getDefinitionValueFromListIgnoreWhiteSpaces(openingLabelNDefinitionList, indexName);
        finalOperand = (int)strtol(definitionValue, &endPointer, DECIMAL);
    }
    if (*endPointer != END_OF_STRING && *endPointer != END_OF_ROW){
        free(binaryWord);
        return NULL;
    }
    binaryOperand = intToBinaryString(finalOperand, MEMORY_WORD_LENGTH_IN_CODE_IMAGE);
    strcpy(binaryWord, binaryOperand);
    free(binaryOperand);
    strcat(binaryWord, ARE);
    return (binaryWord);
}

char* getFixedAddressFirstWordSpaceHolder(){
    return getSpaceHolder();
}

char *getDirectAddressSpaceHolder(){
    return getSpaceHolder();
}

char* getSpaceHolder(){
    char* word = (char*) malloc(IMAGE_WORD_IN_MEMORY_LENGTH * sizeof (char));
    word = NULL;
    return word;
}

char* getImmediateAddressBinaryWord(
        const char* operand, labelOrDefinitionList* openingLabelNDefinitionList){
    char* word = (char*) malloc(IMAGE_WORD_IN_MEMORY_LENGTH * sizeof (char));
    char* ARE = "00";
    char* binaryOperand;
    char *endPointer;
    int finalOperand;
    const char* operandWithoutLadder = &operand[SECOND_CELL_INDEX];
    const char * rawOperand;
    if(is12BitsLegalNumberAsIs(operandWithoutLadder))
        rawOperand = operandWithoutLadder;
    else
        rawOperand = getDefinitionValueFromList(openingLabelNDefinitionList, operandWithoutLadder);
    finalOperand = (int)strtol(rawOperand, &endPointer, DECIMAL);
    if (*endPointer != END_OF_STRING && *endPointer != END_OF_ROW){
        free(word);
        return NULL;
    }
    binaryOperand = intToBinaryString(finalOperand, MEMORY_WORD_LENGTH_IN_CODE_IMAGE);
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
    return memoryWord;
}

char* getBinaryAddressingMethod(addressMethod thisMethod){
    return intToBinaryString(thisMethod, BINARY_ADDRESSING_METHOD_LENGTH);
}

char* getBinaryOpCode(commandOpcode thisCommandOpCode){
    return intToBinaryString(thisCommandOpCode, BINARY_OPCODE_LENGTH);
}








