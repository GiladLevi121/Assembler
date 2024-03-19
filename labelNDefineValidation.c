
#include <string.h>
#include <ctype.h>

#include "labelNDefineValidation.h"
#include "globaldefinitionsNStructures.h"

errorType isLegalTitle(const char* thisTitle, titleTypeErrorIncrement thisTitleType){
    errorType thisError = valid;
    if(isAReservedWord(thisTitle))
        thisError = labelCantBeAKeyWord;
    if(!thisTitle[FIRST_INDEX])
        thisError = firstLabelLetterMustBeAlphabetic;
    if(strlen(thisTitle) > TITLE_MAX_LENGTH)
        thisError = labelsTooLong;
    if(!isTitleCharsAreAllowed(thisTitle))
        thisError = notAllCharactersAreAlphabeticOrNumbers;
    if(thisError == valid)
        return thisError;
    else
        return (thisError + thisTitleType);
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