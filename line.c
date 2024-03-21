
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "globaldefinitionsNStructures.h"
#include "line.h"

assemblyLineCode *assemblyLineCodeConstructor(void){
    assemblyLineCode *newLine = malloc(sizeof (assemblyLineCode));
    return newLine;
}

lineStatus getContentStatus(assemblyLineCode *thisAssemblyLineCode){
    lineStatus thisLineStatus = validLine;
    int counter = ZEROISE_COUNTER;
    int lineLength = strlen(thisAssemblyLineCode->content);
    /*case that there are 80 char + /n in the 81nt => status is valid*/
    if((lineLength == LINE_LENGTH_WITH_N) && (thisAssemblyLineCode->content[lineLength]) == '\n')
        return thisLineStatus;
    if(lineLength > INPUT_LINE_LENGTH)
        thisLineStatus = lineOutOfBounds;
    while (counter <= lineLength){
        if(!isspace(thisAssemblyLineCode->content[counter]))
            return thisLineStatus; /*fond char that isn't "white"*/
        counter++;
    }
    thisLineStatus = emptyLine;
    return thisLineStatus;
}

void setContent(assemblyLineCode* assemblyLine, const char* rawLine){
    int length = FIRST_INDEX;
    int index = FIRST_INDEX;
    /*finds the end of the line: '\0' or '\n'*/
    while (rawLine[length] != END_OF_STRING && rawLine[length] != '\n')
        length++;
    for (; index < length; index++)
        assemblyLine->content[index] = rawLine[index];
    assemblyLine->content[length] = END_OF_STRING;
}

void setStatus(assemblyLineCode* thisAssemblyLine){
    thisAssemblyLine->status = getContentStatus(thisAssemblyLine);
}

void setAssemblyLineCode(assemblyLineCode* assemblyLine, const char* rawLine){
    setContent(assemblyLine, rawLine);
    setStatus(assemblyLine);
}