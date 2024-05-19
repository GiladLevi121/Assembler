
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "preAssembblerMainFile.h"
#include "filesUtil.h"
#include "parser.h"
#include "macroUtiles.h"
#include "labelNDefineValidation.h"


boolean runNReturnStatusOfPreAssembler(char *fileName, macroList* thisMacrosList){
    FILE *filePointerToRead = openFileByName(fileName, AS_ENDING, "r");
    FILE *filePointerToWrite = openFileByName(fileName, AM_ENDING, "w");
    assemblyLineCode *newAssemblyLine;
    int instructionCounter = ZEROISE_COUNTER;
    boolean errorDetect = false;

    if (preAssemblyOpenFileCheckNReturnIsNeeded(filePointerToRead, filePointerToWrite, fileName)){

        return true;
    }

    while ((newAssemblyLine = getNextAssemblyLine(filePointerToRead)) != NULL){
        preAssemblyEveryLineOperations(newAssemblyLine, filePointerToRead, filePointerToWrite,
                                       &instructionCounter, fileName, &errorDetect, thisMacrosList);
    }
    fclose(filePointerToRead);
    fclose(filePointerToWrite);

    if (errorDetect == true){
        char* fileNameWithEnding = connect2strings(fileName, AM_ENDING);
        remove(fileNameWithEnding);
        free(fileNameWithEnding);
    }
    return  errorDetect;
}

boolean preAssemblyOpenFileCheckNReturnIsNeeded(FILE *filePointerToRead,
                                                FILE *filePointerToWrite, char* fileName){
    long fileSize;
    if (filePointerToRead == NULL){
        char * AMEndingFileName = connect2strings(fileName, AM_ENDING);
        printf("Couldn't open file %s. Please make sure that the file name entered (without ending of file)"
               "exist in the project directory.\n", fileName);
        freeNRemoveUnnessaceryInCaseOfOpenFileFaile(AMEndingFileName, filePointerToWrite);
        return true;
    }
    if (filePointerToRead != NULL) {
        char * AMEndingFileName = connect2strings(fileName, AM_ENDING);
        fseek (filePointerToRead, ZERO_CHARACTERS, SEEK_END);
        fileSize = ftell(filePointerToRead);
        if (fileSize == ZERO_CHARACTERS) {
            printf("Please note that the file: %s is an empty file.\n\n", fileName);
            freeNRemoveUnnessaceryInCaseOfOpenFileFaile(AMEndingFileName, filePointerToWrite);
            return true;
        }
    }
    rewind(filePointerToRead);
    return false;
}

void freeNRemoveUnnessaceryInCaseOfOpenFileFaile(char * AMEndingFileName, FILE *filePointerToWrite){
    fclose(filePointerToWrite);
    remove(AMEndingFileName);
    free(AMEndingFileName);
}

void preAssemblyEveryLineOperations(assemblyLineCode* newAssemblyLine, FILE *filePointerToRead,
                                    FILE* filePointerToWrite, int * instructionCounter, const char * fileName,
                                    boolean * errorDetect, macroList* thisMacrosList){
    setAssemblyLineOfCodeStatusInPreAssemblerPass(newAssemblyLine, thisMacrosList);
    instructionCounter++;
    if (newAssemblyLine->status == emptyLine || newAssemblyLine->status == commentLine)
        return;
    else if (newAssemblyLine->status == startOfMacro)
        macroDetectedOperations(newAssemblyLine, filePointerToRead,
                                errorDetect, instructionCounter, fileName, thisMacrosList);
    else if (newAssemblyLine->status == useOfMacro){
        replaceTheMacroNameWithMacroContentNWriteToFile(newAssemblyLine, thisMacrosList, filePointerToWrite);
    }
    else if(newAssemblyLine->status == lineOutOfBounds){
        *errorDetect = true;
        printPreAssemblyError(newAssemblyLine, *instructionCounter, NULL, fileName);
    }else if (newAssemblyLine->status == validLine && *errorDetect == false){
        char* lineToWrite;
        if (feof(filePointerToRead)){
            lineToWrite = newAssemblyLine->content;
            writeStringToFile(filePointerToWrite, lineToWrite);
        }else {
            lineToWrite = connect2strings(newAssemblyLine->content, STRING_END_OF_ROW);
            writeStringToFile(filePointerToWrite, lineToWrite);
            free(lineToWrite);
        }
    }
}

void replaceTheMacroNameWithMacroContentNWriteToFile(assemblyLineCode *newAssemblyLine,
                                         macroList *thisMacrosList, FILE* filePointerToWrite){
    char* macroName = trimLeadingNEndingWhitespace(newAssemblyLine->content);
    char* macroContent = getMacroContentByName(macroName, thisMacrosList);
    writeStringToFile(filePointerToWrite, macroContent);
}


void macroDetectedOperations(assemblyLineCode* thisAssemblyLine, FILE* filePointerToRead,
                             boolean* errorDetect, int* instructionCounter, const char *fileName,
                             macroList* thisMacrosList){
    macroError thisMacroError;
    char* macroName, *macroContent;
    thisMacroError = validMacro;
    macroName = getMacroName(thisAssemblyLine, &thisMacroError);
    macroContent = getMacroContent(thisAssemblyLine, filePointerToRead,
                                         errorDetect, instructionCounter, fileName, &thisMacroError);

    if (thisMacroError == validMacro){
        macroNode * newMacro = macroConstructor(macroName, macroContent);
        addMacroToList(newMacro, thisMacrosList);
    }else
        *errorDetect = true;
    free(macroName);
    free(macroContent);
}

char* getMacroContent(assemblyLineCode* thisAssemblyLine, FILE* filePointerToRead,
                      boolean* errorDetect, int* instructionCounter, const char* fileName,
                      macroError * thisMacroError){
    char* macroContentToBe = (char*) malloc(CHARS_TO_WRITE_10_LINES * sizeof (char));
    strcpy(macroContentToBe, STRING_END_OF_STRING);
    while ((thisAssemblyLine = getNextAssemblyLine(filePointerToRead)) != NULL){
        setAssemblyLineOfCodeStatusInPreAssemblerPass(thisAssemblyLine, NULL);
        instructionCounter ++;
        if (thisAssemblyLine->status == emptyLine || thisAssemblyLine->status == commentLine)
            continue;
        else if (thisAssemblyLine->status == lineOutOfBounds){
            *thisMacroError = errorInInnerLineOfTheMacroContent;
            *errorDetect = true;
            printPreAssemblyError(thisAssemblyLine, *instructionCounter, NULL, fileName);
        }else if (thisAssemblyLine->status == endOfMacro)
            break;
        else if (thisAssemblyLine->status == validLine && *errorDetect == false){
            if (strlen(macroContentToBe) + strlen(thisAssemblyLine->content) + PADDING_CELL_LEN >=
            CHARS_TO_WRITE_10_LINES)
                macroContentToBe = (char*)realloc(macroContentToBe,
                        MEMORY_INCREMENT_FACTOR * strlen(macroContentToBe) * sizeof(char));
            strcat(macroContentToBe, thisAssemblyLine->content);
            strcat(macroContentToBe, STRING_END_OF_ROW);
        }
    }
    return macroContentToBe;
}

char* getMacroName(assemblyLineCode* definedMacroLine, macroError* namingError){
    char* token = getTokenWithoutFirstWordAndFollowedWhiteSpaces(definedMacroLine->content);
    char* macroName = trimLeadingNEndingWhitespace(token);
    if(isNotLegalTitleWithoutBeginningNEndingWhiteSpaces(macroName))
        *namingError = generalErrorInMacroNaming;
    else if(isEmptyString(macroName))
        *namingError = emptyNamingForMacro;
    else if (isAReservedWord(macroName))
        *namingError = macroNameIsAReservedWord;
    if(*namingError != validMacro){
        free(macroName);
        return NULL;
    }
    return macroName;
}

void setAssemblyLineOfCodeStatusInPreAssemblerPass(assemblyLineCode* newAssemblyLine, macroList* thisList){
    setStatusForLineFromAmFile(newAssemblyLine);
    if(newAssemblyLine->status != validLine)
        return;
    else if(isEmptyLine(newAssemblyLine))
        newAssemblyLine->status = emptyLine;
    else if (isCommentLine(newAssemblyLine))
        newAssemblyLine->status = commentLine;
    else if (isBeginningOfMacro(newAssemblyLine))
        newAssemblyLine->status = startOfMacro;
    else if(isEndingOfMacro(newAssemblyLine))
        newAssemblyLine->status = endOfMacro;
    else if (isUsageOfAMacroFromList(newAssemblyLine->content, thisList))
        newAssemblyLine->status = useOfMacro;

}

boolean isEmptyLine(assemblyLineCode *newAssemblyLine){
    return isEmptyString(newAssemblyLine->content);
}

boolean isCommentLine(assemblyLineCode *newAssemblyLine){
    int index;
    index = findFirstNonWhitespaceIndex(newAssemblyLine->content);
    if(newAssemblyLine->content[index] == ';')
        return true;
    return false;
}

boolean isBeginningOfMacro(assemblyLineCode *newAssemblyLine){
    int skipWhiteSpacesIndex = findFirstNonWhitespaceIndex(newAssemblyLine->content);
    char* firstWordOfThisLine = getTokenUpToWhiteSpace(&newAssemblyLine->content[skipWhiteSpacesIndex]);
    if (!strcmp(firstWordOfThisLine, "mcr")){
        free(firstWordOfThisLine);
        return true;
    }
    return false;
}

boolean isEndingOfMacro(assemblyLineCode *newAssemblyLine){
    int skipWhiteSpacesIndex = findFirstNonWhitespaceIndex(newAssemblyLine->content);
    char* firstWordOfThisLine = trimLeadingNEndingWhitespace(&newAssemblyLine->content[skipWhiteSpacesIndex]);
    if (!strcmp(firstWordOfThisLine, "endmcr")){
        free(firstWordOfThisLine);
        return true;
    }
    return false;
}

boolean isUsageOfAMacroFromList(const char* lineContent, macroList* thisList){
    macroNode * current;
    char* potentialMacroName = trimLeadingNEndingWhitespace(lineContent);
    if (thisList == NULL)
        return false;

    current = thisList->head;
    while (current != NULL){
        if (!strcmp(potentialMacroName, current->name)){
            free(potentialMacroName);
            return true;
        }
        current = (macroNode *) current->next;
    }
    free(potentialMacroName);
    return false;
}

void printPreAssemblyError(assemblyLineCode* thisAssemblyLine, int instructionCounter,
                           const macroError* thisMacroError, const char* fileName){
    char* outputFile = connect2strings(fileName, AS_ENDING);
    if(thisAssemblyLine == NULL){
        printf("Error in File:%s, in line:%d. Error details: %s\n",
               outputFile, instructionCounter, stringMacroErrors[*thisMacroError]);
    }else
        printf("Error in File:%s, in line:%d. Error details: Line is too long\n",
               outputFile, instructionCounter); /*only error in line*/
    free(outputFile);
}

