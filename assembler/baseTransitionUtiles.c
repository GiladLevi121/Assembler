
#include "baseTransitionUtiles.h"
#include "globaldefinitionsNStructures.h"
#include <stdio.h>
#include "string.h"
#include <stdlib.h>

/* The order corresponds with each other*/
const char* fourBaseChars = "0123";
const char* encryptedFourBaseChars = "*#%!";

char *charToBinaryString(char ch) {
    char *binaryStr = (char *)malloc((IMAGE_WORD_IN_MEMORY_LENGTH + PADDING_CELL_LEN) * sizeof(char));
    int i;
    binaryStr[IMAGE_WORD_IN_MEMORY_LENGTH] = END_OF_STRING;
    i = IMAGE_WORD_IN_MEMORY_LENGTH - PADDING_CELL_LEN;
    for (; i >= FIRST_INDEX; i--) {
        binaryStr[i] = (ch & CURRENT_BIT) ? '1' : '0';
        ch >>= SHIFT;
    }

    return binaryStr;
}


char *intToBinaryString(int num, int stringLength) {
    unsigned int absNum = abs(num);
    char *binaryStr = (char*)malloc((stringLength + PADDING_CELL_LEN) * sizeof(char));
    int iteration;

    if (binaryStr == NULL)
        return NULL;

    binaryStr[stringLength] = END_OF_STRING;
    for (iteration = stringLength - PADDING_CELL_LEN; iteration >= FIRST_INDEX; iteration--) {
        binaryStr[iteration] = (absNum & CURRENT_BIT) ? '1' : '0';
        absNum >>= SHIFT;
    }
    if (isNegative(num)) /* If the original number is negative => two's complement*/
        complementToTwo(binaryStr, stringLength);
    return binaryStr;
}

boolean isNegative(int number){
    return (number < 0) ? true : false;
}


void complementToTwo(char *binaryStr, int stringLength){
    int carry = CARRY_EXIST;
    int iteration;

    for (iteration = stringLength - PADDING_CELL_LEN; iteration >= FIRST_INDEX; iteration--) {
        if (binaryStr[iteration] == '0')
            binaryStr[iteration] = '1';
        else
            binaryStr[iteration] = '0';
    }

    for (iteration = stringLength - PADDING_CELL_LEN; iteration >= FIRST_INDEX; iteration--) {
        if (binaryStr[iteration] == '0' && carry == CARRY_EXIST) {
            binaryStr[iteration] = '1';
            carry = ZEROISE_CARRY;
        } else if (binaryStr[iteration] == '1' && carry == CARRY_EXIST)
            binaryStr[iteration] = '0';
    }
}


char* memoryWordToEncrypted4Base(const char *thisWord){
    int counter = ZEROISE_COUNTER;
    int index = ZEROISE_COUNTER;
    char* encryptedFourBaseWord = (char*) malloc((
            BASE_FOUR_WORD_LENGTH + PADDING_CELL_LEN + PADDING_CELL_LEN) * sizeof(char));
    for (; counter < IMAGE_WORD_IN_MEMORY_LENGTH; counter += TWO_WORDS_AT_A_TIME) {
        int firstDigit = thisWord[counter] - '0';
        int secondDigit = thisWord[counter + ANOTHER_BIT] - '0';
        int combineDigit = (firstDigit << SHIFT) | secondDigit;
        char fourBaseValue = (char)('0' + combineDigit);
        encryptedFourBaseWord[index++] = getEncryptedCharacter(fourBaseValue);
    }
    encryptedFourBaseWord[index] = END_OF_ROW;
    encryptedFourBaseWord[++index] = END_OF_STRING;
    return encryptedFourBaseWord;
}


char getEncryptedCharacter(char thisCh){
    int endLoop = (int)strlen(fourBaseChars); /*3*/
    int i = ZEROISE_COUNTER;
    for(; i < endLoop; i++){
        if(thisCh == fourBaseChars[i])
            return encryptedFourBaseChars[i];
    }
    return ('4');
}



