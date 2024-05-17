
#include <stdio.h>
#include <stdlib.h>

#include "preAssembblerMainFile.h"
#include "filesUtil.h"
#include "parser.h"


void runPreAssembler(char *fileName){
    FILE *filePointerToRead = openFileByName(fileName, AS_ENDING, "r");
    FILE *filePointerToWrite = openFileByName(fileName, AM_ENDING, "w");
    assemblyLineCode *newAssemblyLine;
    int instructionCounter = ZEROISE_COUNTER;
    if(filePointerToRead == NULL){
        printf("Failed to open file %s, please make sure that file with exact"
               " name is in the project file\n", fileName);
        return;
    }
    while ((newAssemblyLine = getNextAssemblyLine(filePointerToRead)) != NULL){
        /*preAssemblerEveryLineOfAssemblyOperations(newAssemblyLine)*/
        char* stringToWrite;
        if(newAssemblyLine->status == startOfMacro)
            /*addMacroToList*/
        if(newAssemblyLine->status != commentLine)
    }
}

void setAssemblyLineOfCodeStatusInPreAssemblerPass(assemblyLineCode* newAssemblyLine){
    setStatusForLineFromAmFile(newAssemblyLine);
    if(newAssemblyLine->status != validLine){
        return;
    }
    else if(isEmptyLine(newAssemblyLine))
        newAssemblyLine->status = emptyLine;
    else if (isCommentLine(newAssemblyLine))
        newAssemblyLine->status = commentLine;
    else (isBeginingOfMacro(newAssemblyLine))
}

boolean isEmptyLine(assemblyLineCode *newAssemblyLine){
    return isEmptyString(newAssemblyLine->content);
}

boolean isCommentLine(assemblyLineCode *newAssemblyLine){
    int index;
    index = findFirstNonWhitespaceIndexFromEnd(newAssemblyLine->content);
    if(newAssemblyLine->content[index] == ';')
        return true;
    return false;
}


