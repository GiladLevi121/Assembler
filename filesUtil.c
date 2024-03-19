
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "filesUtil.h"
#include "globaldefinitionsNStructures.h"
#include "line.h"

FILE *openFileByName(char *fileName, char *fileEnding, char *permissions) {
    FILE *filePointer;
    char *targetFileName = connectFileNameWithEnding(fileName, fileEnding);
    filePointer = fopen(targetFileName, permissions);
    if (filePointer == NULL) {
        return NULL;
    }
    free(targetFileName);
    return filePointer;
}

char *connectFileNameWithEnding(char *fileName, char *fileEnding) {
    size_t fileLength = strlen(fileName) + strlen(fileEnding);
    char *targetFileName = malloc(fileLength * sizeof(char));
    if (targetFileName != NULL) {
        strcpy(targetFileName, fileName);
        strcat(targetFileName, fileEnding);
        return targetFileName;
    }
    return NULL; /*fileName is empty*/
}

assemblyLineCode *getNextAssemblyLine(FILE *filePointer) {
    char buffer[MAX_CHARS_IN_LINE];
    assemblyLineCode *newAssemblyLineCode = assemblyLineCodeConstructor();
    if (newAssemblyLineCode == NULL) /*failed allocate memory*/
        return NULL;
    if (filePointer == NULL)
        return NULL;
    if (fgets(buffer, sizeof(buffer), filePointer) == NULL)
        return NULL;

    setAssemblyLineCode(newAssemblyLineCode, buffer);
    return newAssemblyLineCode;
}
