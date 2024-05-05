
#include "baseTransitionUtiles.h"
#include "globaldefinitionsNStructures.h"
#include <stdio.h>
#include <stdlib.h>


char *intToBinaryString(int num, int stringLength) {
    unsigned int absNum = abs(num);
    char *binaryStr = (char*)malloc((stringLength + LAST_CELL) * sizeof(char));
    binaryStr[stringLength] = END_OF_STRING;
    int iteration;

    if (binaryStr[FIRST_INDEX] == END_OF_STRING)
        return NULL;

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
    for (iteration = stringLength; iteration >= FIRST_INDEX; iteration--) {
        if (binaryStr[iteration] == '0')
            binaryStr[iteration] = '1';
        else
            binaryStr[iteration] = '0';
    }
    for (iteration = ZEROISE_COUNTER; iteration <= stringLength; iteration++) {
        if (binaryStr[iteration] == '0' && carry == CARRY_EXIST) {
            binaryStr[iteration] = '1';
            carry = ZEROISE_CARRY;
        } else if (binaryStr[iteration] == '1' && carry == CARRY_EXIST)
            binaryStr[iteration] = '0';
    }
}

/*
 *  int carry = CARRY_EXIST;
    int iteration;
    for (iteration = stringLength; iteration >= FIRST_INDEX; iteration--) {
        if (binaryStr[iteration] == '0')
            binaryStr[iteration] = '1';
        else
            binaryStr[iteration] = '0';
    }
    for (iteration = ZEROISE_COUNTER; iteration <= stringLength; iteration++) {
        if (binaryStr[iteration] == '0')
            binaryStr[iteration] = '1';
        else{
            binaryStr[iteration] = '0';

        }
    }*/