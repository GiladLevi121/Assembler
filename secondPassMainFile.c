
#include "secondPassMainFile.h"
#include "memoryImage.h"
#include "globaldefinitionsNStructures.h"
#include "label.h"
#include "filesUtil.h"

void runSecondPass(memoryImage* fileMemoryImage, boolean errorExist,
                   labelOrDefinitionList* openingLabelNDefinitionList,
                   labelOrDefinitionList* entryExternLabelList, char *fileName){
    FILE *filePointer = openFileByName(fileName, AS_ENDING, "r");
    assemblyLineCode *newAssemblyLine;
    int instructionCounter = ZEROISE_COUNTER;
    if(filePointer == NULL){
        printf("Failed to open file %s, please make sure that file with exact"
               " name is in the project file\n", fileName);
        return;
    }
    while ((newAssemblyLine = getNextAssemblyLine(filePointer)) != NULL){
        secondPassEveryLineOfAssemblyOperations(newAssemblyLine,
                                               ++instructionCounter,
                                               openingLabelNDefinitionList,
                                               entryExternLabelList,
                                               fileMemoryImage);
    }

    fclose(filePointer);
}


void secondPassEveryLineOfAssemblyOperations(assemblyLineCode *newAssemblyLine, int instructionCounter,
                                             labelOrDefinitionList* openingLabelNDefinitionList,
                                             labelOrDefinitionList* entryExternLabelList,
                                             memoryImage* fileMemoryImage){

}