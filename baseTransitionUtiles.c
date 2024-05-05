
#include "baseTransitionUtiles.h"
#include "globaldefinitionsNStructures.h"
#include <stdio.h>
#include <stdlib.h>


char *intToBinaryString(int num, int stringLength) {
    unsigned int absNum = abs(num);
    int counter = ZEROISE_COUNTER;
    char *binaryStr = (char*)malloc((stringLength) * sizeof(char));
    binaryStr[stringLength] = END_OF_STRING;

    if (binaryStr == NULL)
        return NULL;

    while (absNum > POSITIVE){
        if(counter == stringLength){
            free(binaryStr);
            return NULL;
        }
        binaryStr[counter] = (char)(absNum % BINARY_MODULO);
        absNum = absNum / BINARY_MODULO;
        counter++;
    }
    while(counter < stringLength){
        binaryStr[counter] = '0';
        counter++;
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