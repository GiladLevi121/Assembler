
#include "baseTransitionUtiles.h"
#include "globaldefinitionsNStructures.h"
#include <stdio.h>
#include <stdlib.h>

char *charToBinaryString(char ch) {
    char *binaryStr = (char *)malloc(IMAGE_WORD_IN_MEMORY_LENGTH * sizeof(char));
    binaryStr[IMAGE_WORD_IN_MEMORY_LENGTH] = END_OF_STRING;
    int i = IMAGE_WORD_IN_MEMORY_LENGTH - LAST_CELL;
    for (; i >= FIRST_INDEX; i--) {
        binaryStr[i] = (ch & CURRENT_BIT) ? '1' : '0';
        ch >>= SHIFT;
    }

    return binaryStr;
}


char *intToBinaryString(int num, int stringLength) {
    unsigned int absNum = abs(num);
    char *binaryStr = (char*)malloc((stringLength + LAST_CELL) * sizeof(char));
    int iteration;

    if (binaryStr == NULL)
        return NULL;

    binaryStr[stringLength] = END_OF_STRING;
    for (iteration = stringLength - LAST_CELL; iteration >= FIRST_INDEX; iteration--) {
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

    for (iteration = stringLength - LAST_CELL; iteration >= FIRST_INDEX; iteration--) {
        if (binaryStr[iteration] == '0')
            binaryStr[iteration] = '1';
        else
            binaryStr[iteration] = '0';
    }

    for (iteration = stringLength - LAST_CELL; iteration >= FIRST_INDEX; iteration--) {
        if (binaryStr[iteration] == '0' && carry == CARRY_EXIST) {
            binaryStr[iteration] = '1';
            carry = ZEROISE_CARRY;
        } else if (binaryStr[iteration] == '1' && carry == CARRY_EXIST)
            binaryStr[iteration] = '0';
    }
}

