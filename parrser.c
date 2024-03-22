
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "parrser.h"
#include "globaldefinitionsNStructures.h"


int findFirstNonWhitespaceIndexFromEnd(const char* str) {
    size_t stringLength = strlen(str);
    int counter = stringLength;
    for (; counter != FIRST_INDEX || str[counter] != END_OF_STRING; --counter) {
        if (!isspace(str[counter])) {
            return counter;
        }
    }
    return ALL_WHITE_SPACES;
}

int findFirstNonWhitespaceIndex(const char* str) {
    int counter = FIRST_INDEX;
    for (; str[counter] != END_OF_STRING  ; ++counter) {
        if (!isspace(str[counter])) {
            return counter;
        }
    }
    return ALL_WHITE_SPACES;
}

boolean twoConsecutiveCommas(const char *line) {
    int i;
    boolean commaExist = false;
    for (i = FIRST_INDEX; line[i] != END_OF_STRING; i++) {
        if (line[i] == ',') {
            if (commaExist == true)  /*TWO COMMA IN A ROW*/
                return true;
            else
                commaExist = true;
        } else if (!isspace(line[i])) /*ignoring space char*/
            commaExist = false;
    }
    return commaExist;
}

boolean endsWithComma(const char * line){
    size_t counter = ZEROISE_COUNTER;

    if(line == NULL || line[counter] == END_OF_STRING)
        return false;

    counter = strlen(line);
    while((counter >= FIRST_INDEX) && (isspace(line[counter])))
        counter--;

    if(line[counter] == ',')
        return true;

    return false;
}

boolean isEndOfLine(const char * line){
    if(line[FIRST_INDEX] == END_OF_STRING)
        return true;
    return false;
}

boolean isThereMandatoryWhiteSpace(const char* line){
    if(isspace(line[FIRST_INDEX]))
        return true;
    return false;
}

char * getTokensUpToChar(const char* str, char knot){
    char *token = (char*)malloc(sizeof (char) * MAX_CHARS_IN_LINE);
    size_t tokenLength = ZEROISE_COUNTER;
    if (strlen(str) == ZEROISE_COUNTER || str[FIRST_INDEX] == END_OF_STRING)
        return NULL;
    while (!isEndOfLine(&str[tokenLength])){
        while(str[tokenLength] != knot)
            tokenLength++;
        break;
    }
    if(tokenLength == ZEROISE_COUNTER)
        return NULL;

    memcpy(token, str, tokenLength);
    token[tokenLength] = END_OF_STRING;
    return token;
}

char* getTokenUpToWhiteSpace(const char* str){
    char *token = (char*)malloc(sizeof (char) * MAX_CHARS_IN_LINE);
    int counter = ZEROISE_COUNTER;

    while(counter < MAX_CHARS_IN_LINE){
        if(str[counter] == END_OF_STRING || isspace(str[counter]))
            break;
        if(!isspace(str[counter])){
            /*if (counter == )*/
            token[counter] = str[counter];
            token[counter + ANOTHER_CELL] = END_OF_STRING;
        }
        counter++;
    }
    return token;
}

boolean isLegalXBitsNumber(const char* potentialNumber, int minRang, int maxRang){
    int beginning = findFirstNonWhitespaceIndex(potentialNumber);
    char *endingPointer;
    int counter = ZEROISE_COUNTER;
    long thisNumber;
    thisNumber = strtol(&potentialNumber[beginning], &endingPointer, DECIMAL);
    if (potentialNumber[beginning] == '+' || potentialNumber[beginning] == '-') {
        if (potentialNumber[beginning + LAST_CELL] == END_OF_STRING) {
            return END_OF_STRING;
        }
        potentialNumber++;
    }
    while (*potentialNumber != END_OF_STRING) {
        if (!isdigit(potentialNumber[counter + beginning])) {
            return false;
        }
        counter++;
    }
    if (thisNumber >= minRang && thisNumber <= maxRang) {
        return true;
    }
    return false;
}

boolean is14BitsLegalNumber(const char* potentialNumber){
    return isLegalXBitsNumber(potentialNumber,
                              FOURTEEN_BITS_MIN_NUMBER,
                              FOURTEEN_BITS_MAX_NUMBER);
}

boolean is12BitsLegalNumber(const char* potentialNumber){
    return isLegalXBitsNumber(potentialNumber,
                              TWELVE_BITS_MIN_NUMBER,
                              TWELVE_BITS_MAX_NUMBER);
}

void trimLeadingNEndingWhitespaceFromStr(char *str) {
    int start = FIRST_INDEX;
    size_t end = strlen(str) - LAST_CELL;
    size_t length;

    /*Trim leading white spaces*/
    while (isspace((unsigned char) str[start]))
        start++;

    /*Trim trailing white spaces*/
    while (end > start && isspace((unsigned char) str[end]))
        end--;

    /*Shift the remaining characters to the beginning of the string*/
    length = end - start + 1;
    memmove(str, str + start, length);
    str[length] = END_OF_STRING;
}


















