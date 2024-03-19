
#include <stdlib.h>
#include <stdio.h>


#include "firstPassMainFile.h"
#include "filesUtil.h"
#include "globaldefinitionsNStructures.h"
#include "line.h"
#include "lexTree.h"

void runFirstPass(char *fileName,
                  labelOrDefinitionList* openingLabelNDefinitionList,
                  labelOrDefinitionList* definitionList){
    FILE *filePointer = openFileByName(fileName, AS_ENDING, "r");
    assemblyLineCode *newAssemblyLine;
    int programCounter = ZEROISE_COUNTER;
    if(filePointer == NULL){
        printf("Failed to open file %s, please make sure that file with exact"
               " name is in the project file", fileName);
        return;
    }
    while ((newAssemblyLine = getNextAssemblyLine(filePointer)) != NULL){
        lexTree *thisLexTree = lexTreeConstructor(newAssemblyLine, ++programCounter);
        /*lexTreeInnerValidation*/
        listsUpdating(openingLabelNDefinitionList, thisLexTree);
        /*codingToImage*/
        free(newAssemblyLine);
        free(thisLexTree);
    }



    fclose(filePointer);s
    /*deallocateLabelListElements(labelList);
    deallocateLabelListElements(definitionList);*/
}

void listsUpdating(labelOrDefinitionList* labelNDefinitionList, lexTree* thisLexTree){
    if(thisLexTree->potentialLabel != NULL){
        addLabelNodeAtTheEnd(labelNDefinitionList, thisLexTree->potentialLabel);
    }
}