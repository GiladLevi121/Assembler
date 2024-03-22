

#include <string.h>

#include "globaldefinitionsNStructures.h"

/**/
const char* reservedAssemblyWords[] = {"mov", "cmp","add","sub",
                                       "not","clr","lea","inc",
                                       "dec","jmp","bne","red",
                                       "prn","jsr","rts","hlt",
                                       "string","data","entry",
                                       "define","extern"};

boolean isAReservedWord(const char* inputString){
    size_t numberOfReservedWords = sizeof(reservedAssemblyWords) /
            sizeof(reservedAssemblyWords[FIRST_INDEX]);
    int counter = ZEROISE_COUNTER;
    for(; counter < numberOfReservedWords; ++counter){
        if(strcmp(inputString, reservedAssemblyWords[counter]) == EQUAL_STRINGS)
            return true;
    }
    return false;
}



