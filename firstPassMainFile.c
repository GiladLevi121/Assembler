
#include <stdlib.h>
#include <stdio.h>


#include "firstPassMainFile.h"
#include "filesUtil.h"
#include "globaldefinitionsNStructures.h"
#include "line.h"
#include "lexTree.h"
#include "lexTreeValidation.h"
#include "label.h"

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
        lexTree *thisLexTree = lexTreeConstructor(newAssemblyLine, ++programCounter);
        validateLexTree(thisLexTree);
        /*listsUpdating(openingLabelNDefinitionList, entryNExternalList, thisLexTree);*/

        /*codingToImage*/
        free(newAssemblyLine);
        free(thisLexTree);
    }


    fclose(filePointer);
    /*deallocateLabelListElements(openingLabelNDefinitionList);*/

}

void listsUpdating(labelOrDefinitionList* labelNDefinitionList, labelOrDefinitionList* entryNExternalList, lexTree* thisLexTree){
    if(thisLexTree->potentialLabel != NULL)
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
            /*addLabelOrDefinitionNodeAtTheEnd(entryNExternalList, )*/
}