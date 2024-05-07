

#include <string.h>

#include "globaldefinitionsNStructures.h"

/**/
const char* reservedAssemblyWords[] = {"mov", "cmp","add","sub",
                                       "not","clr","lea","inc",
                                       "dec","jmp","bne","red",
                                       "prn","jsr","rts","hlt",
                                       "string","data","entry",
                                       "define","extern", "r0",
                                       "r1", "r2", "r3","r4",
                                       "r5","r6","r7", ".string",
                                       ".data",".entry",
                                       ".define",".extern"};



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

boolean isARegisterName(const char * str){
    int i = ZEROISE_COUNTER;
    for(; i < REGISTERS_AMOUNT; i++){
        if(!strcmp(str, reservedAssemblyWords[REGISTERS_FIRST_INDEX_IN_KEY_WORDS + i]))
            return true;
    }
    return false;
}


void appendToken(char* dest, const char* token){
    size_t orgLength = strlen(dest);
    int counter = ZEROISE_COUNTER;
    for(;counter < strlen(token); counter++){
        dest[orgLength + counter] = token[counter];
    }
    dest[orgLength + counter] = END_OF_STRING;
}


