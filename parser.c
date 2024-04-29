
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "parser.h"
#include "globaldefinitionsNStructures.h"


int findFirstNonWhitespaceIndexFromEnd(const char* str) {
    size_t stringLength = strlen(str);
    int counter = (int)stringLength;
    while (counter != FIRST_INDEX || str[counter] != END_OF_STRING) {
        if (!isspace(str[counter]) && counter < stringLength) {
            return counter;
        }
        counter--;
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
    if(strchr(str, knot) == NULL)
        return NULL;
    if (strlen(str) == ZEROISE_COUNTER || str[FIRST_INDEX] == END_OF_STRING)
        return NULL;
    while (!isEndOfLine(&str[tokenLength])){
        while(str[tokenLength] != knot)
            tokenLength++;
        break;
    }
    if(tokenLength == strlen(str))
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

boolean isLegalXBitsNumber(const char* potentialNumber, int minRang, int maxRang, boolean ignoreFlag){
    int beginning, endingIndex, counter = ZEROISE_COUNTER;
    char *endingPointer;
    long thisNumber;
    if(ignoreFlag){/*true <=> ignore beginning and ending white spaces*/
        beginning = findFirstNonWhitespaceIndex(potentialNumber);
        endingIndex = findFirstNonWhitespaceIndexFromEnd(potentialNumber);
    }else{
        beginning = ZEROISE_COUNTER;
        endingIndex = (int)strlen(potentialNumber) - LAST_CELL;
    }
    thisNumber = strtol(&potentialNumber[beginning], &endingPointer, DECIMAL);
    if (potentialNumber[beginning] == '+' || potentialNumber[beginning] == '-') {
        if (potentialNumber[beginning + LAST_CELL] == END_OF_STRING)
            return END_OF_STRING;
        potentialNumber++;
    }
    while (potentialNumber[counter + beginning] != END_OF_STRING && (counter + beginning <= endingIndex )) {
        if (!isdigit(potentialNumber[counter + beginning]))
            return false;
      counter++;
    }
    if (thisNumber >= minRang && thisNumber <= maxRang)
        return true;
    return false;
}

boolean is14BitsLegalNumberIgnoreWhiteSpaces(const char* potentialNumber){
    return isLegalXBitsNumber(potentialNumber,
                              FOURTEEN_BITS_MIN_NUMBER,
                              FOURTEEN_BITS_MAX_NUMBER, true);
}

boolean is12BitsLegalNumberIgnoreWhiteSpaces(const char* potentialNumber){
    return isLegalXBitsNumber(potentialNumber,
                              TWELVE_BITS_MIN_NUMBER,
                              TWELVE_BITS_MAX_NUMBER, true);
}

boolean is14BitsLegalNumberAsIs(const char* potentialNumber){
    return isLegalXBitsNumber(potentialNumber,
                              FOURTEEN_BITS_MIN_NUMBER,
                              FOURTEEN_BITS_MAX_NUMBER, false);
}

boolean is12BitsLegalNumberAsIs(const char* potentialNumber){
    return isLegalXBitsNumber(potentialNumber,
                              TWELVE_BITS_MIN_NUMBER,
                              TWELVE_BITS_MAX_NUMBER, false);
}


char* trimLeadingNEndingWhitespace(const char* str) {
    size_t start = FIRST_INDEX;
    size_t len = strlen(str);
    size_t end = len - LAST_CELL;
    size_t trimmedLen = end - start + ANOTHER_CELL;
    char* trimmedStr = (char*)malloc((trimmedLen + ANOTHER_CELL) * sizeof(char));
    if (str == NULL)
        return NULL;
    if (len == ZERO_CHARACTERS)
        return "";
    while (isspace(str[start]))
        start++;
    while (end > start && isspace(str[end]))
        end--;
    trimmedLen = end - start + ANOTHER_CELL;
    memcpy(trimmedStr, &str[start], trimmedLen);
    trimmedStr[trimmedLen] = END_OF_STRING;

    return trimmedStr;
}

boolean isValidAssemblyArray(const char * str, int beginning, int ending){

}

boolean isEmptyString(const char *str) {
    if (str == NULL)
        return true;
    while (*str != '\0') {
        if (!isspace(*str))
            return false;
        str++;
    }
    return true;
}
















