
#include <stdlib.h>
#include <stdio.h>


#include "firstPassMainFile.h"
#include "filesUtil.h"
#include "globaldefinitionsNStructures.h"
#include "line.h"
#include "lexTree.h"
#include "lexTreeValidation.h"
#include "label.h"
#include "codingToCodeImageFirstPass.h"

void runFirstPass(char *fileName,
                  labelOrDefinitionList* openingLabelNDefinitionList,
                  labelOrDefinitionList* entryNExternalList){
    FILE *filePointer = openFileByName(fileName, AS_ENDING, "r");
    assemblyLineCode *newAssemblyLine;
    int programCounter = ZEROISE_COUNTER;
    if(filePointer == NULL){
        printf("Failed to open file %s, please make sure that file with exact"
               " name is in the project file", fileName);
        return;
    }
    while ((newAssemblyLine = getNextAssemblyLine(filePointer)) != NULL){
        firstPassEveryLineOfAssemblyOperations(newAssemblyLine, programCounter, openingLabelNDefinitionList,
                            entryNExternalList);
    }

    fclose(filePointer);
    /*deallocateLabelListElements(openingLabelNDefinitionList);*/

}


void firstPassEveryLineOfAssemblyOperations(assemblyLineCode *newAssemblyLine, int programCounter,
                         labelOrDefinitionList* openingLabelNDefinitionList,
                         labelOrDefinitionList* entryNExternalList){
    lexTree *thisLexTree = lexTreeConstructor(newAssemblyLine, ++programCounter);
    validateLexTree(thisLexTree);
    listsUpdating(openingLabelNDefinitionList, entryNExternalList, thisLexTree);

    codingThisLexTree(thisLexTree);

    free(newAssemblyLine);
    free(thisLexTree);
}

void listsUpdating(labelOrDefinitionList* labelNDefinitionList, labelOrDefinitionList* entryNExternalList, lexTree* thisLexTree){
    /*if(thisLexTree->potentialLabel != NULL)
        addLabelOrDefinitionNodeAtTheEnd(labelNDefinitionList, thisLexTree->potentialLabel);
    if (thisLexTree->type == definition){
        labelOrDefinitionNode * newDefinitionNode = definitionNodeConstructor(
                thisLexTree->content.definitionContent.name, thisLexTree->content.definitionContent.value);
        addLabelOrDefinitionNodeAtTheEnd(labelNDefinitionList, newDefinitionNode);
        return;
    }
    if(thisLexTree->type == direction)
        if(thisLexTree->content.directionSentence.type == entryDirection ||
           thisLexTree->content.directionSentence.type == externDirection)
            printf("hi");
            addLabelOrDefinitionNodeAtTheEnd(entryNExternalList, )*/
}



void codingThisLexTree(lexTree* thisLexTree){
    if(thisLexTree->type == order)
        codeToCodeImage(thisLexTree);
}
























