
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "labelNDefineValidation.h"
#include "globaldefinitionsNStructures.h"
#include "parser.h"

errorType isNotLegalTitleWithoutBeginningNEndingWhiteSpaces(const char* potentialTitle){
    char * rawPotentialTitle = trimLeadingNEndingWhitespace(potentialTitle);
    errorType thisError = isNotLegalTitle(rawPotentialTitle);
    free(rawPotentialTitle);
    return thisError;
}

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
    size_t labelLength = strlen(thisTitle) /*+ PADDING_CELL_LEN*/;
    for(; counter < labelLength /*|| thisTitle[counter] != END_OF_STRING*/; counter++){
        if(!isalpha(thisTitle[counter]) && !isdigit(thisTitle[counter]))
            return false;
    }
    return true;
}