
#include <stdlib.h>
#include <stdio.h>


#include "firstPassMainFile.h"
#include "filesUtil.h"
#include "globaldefinitionsNStructures.h"
#include "line.h"
#include "SettingLexTree.h"
#include "lexTreeValidation.h"
#include "label.h"
#include "firstPassCoding.h"

void runFirstPass(char *fileName,
                  labelOrDefinitionList* openingLabelNDefinitionList,
                  labelOrDefinitionList* entryNExternalList){
    FILE *filePointer = openFileByName(fileName, AS_ENDING, "r");
    assemblyLineCode *newAssemblyLine;
    int instructionCounter = ZEROISE_COUNTER;
    if(filePointer == NULL){
        printf("Failed to open file %s, please make sure that file with exact"
               " name is in the project file", fileName);
        return;
    }
    while ((newAssemblyLine = getNextAssemblyLine(filePointer)) != NULL){
        firstPassEveryLineOfAssemblyOperations(newAssemblyLine,
                                               ++instructionCounter,
                                               openingLabelNDefinitionList,
                                               entryNExternalList);
    }
    fclose(filePointer);
    deallocateLabelListElements(openingLabelNDefinitionList);
}


void firstPassEveryLineOfAssemblyOperations(assemblyLineCode *newAssemblyLine, int instructionCounter,
                         labelOrDefinitionList* openingLabelNDefinitionList,
                         labelOrDefinitionList* entryNExternalList){
    lexTree *thisLexTree = lexTreeConstructor(newAssemblyLine, instructionCounter);
    validateLexTree(thisLexTree, openingLabelNDefinitionList);
    listsUpdating(openingLabelNDefinitionList, entryNExternalList, thisLexTree);
    codingThisLexTree(thisLexTree, openingLabelNDefinitionList);
    //printf("Line of assembly: %d.    error type: %d\n", instructionCounter, thisLexTree->error);
    free(newAssemblyLine);
    freeLexTree(thisLexTree);
}

void listsUpdating(labelOrDefinitionList* labelNDefinitionList,
                   labelOrDefinitionList* entryNExternalList, lexTree* thisLexTree){
    if(thisLexTree->error != valid)
        return;
    if(thisLexTree->potentialLabel != NULL){
        addLabelOrDefinitionNodeAtTheEnd(labelNDefinitionList, thisLexTree->potentialLabel);
    }
    if (thisLexTree->type == definition){
        labelNode * newDefinitionNode = labelDefinitionNodeConstructor(
                thisLexTree->content.definitionContent.name,
                thisLexTree->content.definitionContent.value);
        addLabelOrDefinitionNodeAtTheEnd(labelNDefinitionList, newDefinitionNode);
        return;
    }
    /*
    if(thisLexTree->type == direction)
        if(thisLexTree->content.directionSentence.type == entryDirection ||
           thisLexTree->content.directionSentence.type == externDirection)
            printf("hi");*/
            /*addLabelOrDefinitionNodeAtTheEnd(entryNExternalList, )*/
}



void codingThisLexTree(lexTree* thisLexTree, labelOrDefinitionList *openingLabelNDefinitionList){
    int amountOfWordsToCode = ZEROISE_COUNTER, i = 0;
    char** wordsToCode;
    printf("Line number: %d:  %s\n", thisLexTree->InstructionCounter, thisLexTree->rawLine->content);
    if(thisLexTree->error != valid)
        return;
    if(thisLexTree->type == order){
        wordsToCode = getBinaryRepresentationOfThisOrder(thisLexTree,
                                                         &amountOfWordsToCode ,openingLabelNDefinitionList);
        setbuf(stdout,0);
        for(; i < amountOfWordsToCode; i++){
            if(wordsToCode[i] != NULL){
                printf("%s\n", wordsToCode[i]);
                free(wordsToCode[i]);
            }else{
                printf("NULL\n");
            }
        }
        free(wordsToCode);
        printf("\n");
    }else if(thisLexTree->content.directionSentence.type == dataDirection) {
        wordsToCode = getBinaryRepresentationOfDirection(thisLexTree, &amountOfWordsToCode,
                                                         openingLabelNDefinitionList);
        for (; i < getArgumentAmountInDataContent(thisLexTree); i++) {
            printf("%s\n", wordsToCode[i]);
            free(wordsToCode[i]);
        }
        free(wordsToCode);
        printf("\n");
    }else if (thisLexTree->content.directionSentence.type == stringDirection){
        wordsToCode = getBinaryRepresentationOfDirection(thisLexTree, &amountOfWordsToCode,
                                                         openingLabelNDefinitionList);
        for (; i < amountOfWordsToCode; i++) {
            printf("%s\n", wordsToCode[i]);
            free(wordsToCode[i]);
        }
        free(wordsToCode);
        printf("\n");
    }
    printf("\n");
}







/*
labelNode *current = openingLabelNDefinitionList->head;
while(current->next != NULL){
    printf("%s  \n",current->title);
    current = (labelNode *) current->next;
}
printf("%s  \n",current->title);
*/











