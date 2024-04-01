
#include <string.h>
#include <ctype.h>

#include "labelNDefineValidation.h"
#include "globaldefinitionsNStructures.h"

errorType isNotLegalTitle(const char* thisTitle){
    errorType thisError = valid;
    if(isAReservedWord(thisTitle))
        thisError = titleOfLabelOrDefinitionCantBeAKeyWord;
    if(!isalpha(thisTitle[FIRST_INDEX]))
        thisError = firstLetterOfDefinitionOrLabelTitleMustBeAlphabetic;
    if(strlen(thisTitle) > TITLE_MAX_LENGTH)
        thisError = labelOrDefinitionTitleTooLong;
    if(!isTitleCharsAreAllowed(thisTitle))
        thisError = notAllCharactersAreAlphabeticOrNumbers;
    return thisError;
}

boolean isTitleCharsAreAllowed(const char* thisTitle){
    int counter = ZEROISE_COUNTER;
    size_t labelLength = strlen(thisTitle);
    for(; counter < labelLength; counter++){
        if(!isalpha(thisTitle[counter]) && !isdigit(thisTitle[counter]))
            return false;
    }
    return true;
}