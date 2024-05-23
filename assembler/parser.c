
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "parser.h"
#include "globaldefinitionsNStructures.h"


int findFirstNonWhitespaceIndexFromEnd(const char* str) {
    size_t stringLength = strlen(str);
    int counter = (int)stringLength - PADDING_CELL_LEN;

    while (counter >= ZEROISE_COUNTER && isspace(str[counter])) {
        counter--;
    }

    return counter;
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
    int counter = ZEROISE_COUNTER;

    if(line == NULL || line[counter] == END_OF_STRING)
        return false;

    counter = (int)strlen(line);
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
    if(strchr(str, knot) == NULL){
        free(token);
        return NULL;
    }
    if (strlen(str) == ZEROISE_COUNTER || str[FIRST_INDEX] == END_OF_STRING){
        free(token);
        return NULL;
    }
    while (!isEndOfLine(&str[tokenLength])){
        while(str[tokenLength] != knot)
            tokenLength++;
        break;
    }
    if(tokenLength == strlen(str))
        return NULL;

    strncpy(token, str, tokenLength + PADDING_CELL_LEN);
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
            token[counter + PADDING_CELL_LEN] = END_OF_STRING;
        }
        counter++;
    }
    return token;
}

char* getTokenWithoutFirstWordAndFollowedWhiteSpaces(const char* str){
    char *token = (char*)malloc(sizeof (char) * MAX_CHARS_IN_LINE);
    int firstNonWhiteSpaceIndex = findFirstNonWhitespaceIndex(str);
    int counter = ZEROISE_COUNTER;
    while (!isspace(str[firstNonWhiteSpaceIndex + counter])){
        if(firstNonWhiteSpaceIndex + counter == strlen(str))
            return NULL;
        counter++;
    }
    strcpy(token, &str[firstNonWhiteSpaceIndex + counter]);
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
        endingIndex = (int)strlen(potentialNumber) - PADDING_CELL_LEN;
    }
    thisNumber = strtol(&potentialNumber[beginning], &endingPointer, DECIMAL);
    if (potentialNumber[beginning] == '+' || potentialNumber[beginning] == '-') {
        if (potentialNumber[beginning + PADDING_CELL_LEN] == END_OF_STRING)
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
    int strLength, start = ZEROISE_COUNTER;
    int  end = ZEROISE_COUNTER;
    char* trimmedStr = NULL;
    if (str == NULL)
        return NULL;
    if (isEmptyString(str))
        return "";
    if(str != NULL)
        strLength = (int)strlen(str);
    while (isspace(str[start]) && start < strLength) {
        start++;
    }
    while (isspace(str[strLength - end - PADDING_CELL_LEN])) {
        end++;
    }
    if(strLength - start - end < ZERO_CHARACTERS)
        return NULL;
    trimmedStr = (char*)malloc((strLength + PADDING_CELL_LEN) * sizeof(char));
    strncpy(trimmedStr, &str[start], (strLength - start - end));
    trimmedStr[(strLength - start - end)] = END_OF_STRING;
    return trimmedStr;
}

boolean isEmptyString(const char *str) {
    if (str == NULL)
        return true;
    while (*str != END_OF_STRING) {
        if (!isspace(*str))
            return false;
        str++;
    }
    return true;
}
















