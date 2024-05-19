

#include <string.h>
#include <stdio.h>

#include "globaldefinitionsNStructures.h"



const char* reservedAssemblyWords[] = {"mov", "cmp","add","sub",
                                       "not","clr","lea","inc",
                                       "dec","jmp","bne","red",
                                       "prn","jsr","rts","hlt",
                                       "string","data","entry",
                                       "define","extern", "r0",
                                       "r1", "r2", "r3","r4",
                                       "r5","r6","r7", ".string",
                                       ".data",".entry",
                                       ".define",".extern", "mcr", "endmcr"};


const char* errorStings[] = {
        "valid",
        "First letter of definition or label title must be alphabetic",
        "Label or definition title too long",
        "Title of label or definition cant be a key word",
        "Not all characters are alphabetic or numbers",
        "Mandatory white char after key word",
        "Undefined command",
        "Missing argument",
        "Undefined direction",
        "Missing argument after direction",
        "Source operand address method error",
        "Destination operand address method error",
        "Missing operand",
        "Incompatible operand",
        "To manny operands",
        "Incompatible argument for data declaration",
        "Label name is already used as macro name",
        "Invalid name for entry or extern declaration",
        "Label title alreadyUsed",
        "Definition cant Have a label",
        "Comma is the last char",
        "Comma can't be the firs char after direction declaration",
        "Missing quotation marks in string declaration",
        "Forbidden use of a reserved word",
        "Illegal number",
        "Definition naming is illegal",
        "Missing equal knot in define sentence",
        "Declared entry label but didn't declared label in file (Error also appears if label declared in file, but the label or the line it declared are illegal from some reason).",
        "Unknown label usage (Error also appears if label declared in file, but the label or the line it declared are illegal from some reason).",
        "Can't use external declaration name to already exist label or definition",
        "Entry or extern declaration cant appear twice",
        "Can't use entry declaration to refer to definition name",
        "Two consecutive commas found",
        "Line length is too long"
};

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



void printErrorToUser(const char* outputFile, errorType thisError, int instructionCounter){
    printf("Error in file:%s, in line:%d. Error details: %s\n",
           outputFile, instructionCounter, errorStings[thisError]);
}

