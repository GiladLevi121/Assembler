
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
#include "memoryImage.h"

void runFirstPass(char *fileName,
                  labelOrDefinitionList* openingLabelNDefinitionList,
                  labelOrDefinitionList* entryNExternalList,
                  memoryImage* fileMemoryImage){
    FILE *filePointer = openFileByName(fileName, AS_ENDING, "r");
    assemblyLineCode *newAssemblyLine;
    int instructionCounter = ZEROISE_COUNTER;
    if(filePointer == NULL){
        printf("Failed to open file %s, please make sure that file with exact"
               " name is in the project file\n", fileName);
        return;
    }
    while ((newAssemblyLine = getNextAssemblyLine(filePointer)) != NULL){
        firstPassEveryLineOfAssemblyOperations(newAssemblyLine,
                                               ++instructionCounter,
                                               openingLabelNDefinitionList,
                                               entryNExternalList,
                                               fileMemoryImage);
    }
    dataImageEndOfFirstPassUpdating(fileMemoryImage, openingLabelNDefinitionList);
    labelNode *current = openingLabelNDefinitionList->head;
    while(current->next != NULL){
        if (current->labelType == code ||
        current->labelType == data)
            printf("%s  %s\n", current->title, current->value.PC);
        else
            printf("%s  %s\n", current->title, current->value.definitionValue);
        current = (labelNode *) current->next;
    }
    if (current->labelType == code ||
        current->labelType == data)
        printf("%s  %s\n", current->title, current->value.PC);
    else
        printf("%s  %s\n", current->title, current->value.definitionValue);
    fclose(filePointer);

}


void firstPassEveryLineOfAssemblyOperations(assemblyLineCode *newAssemblyLine, int instructionCounter,
                         labelOrDefinitionList* openingLabelNDefinitionList,
                         labelOrDefinitionList* entryNExternalList,
                         memoryImage* fileMemoryImage){
    lexTree *thisLexTree = lexTreeConstructor(newAssemblyLine, instructionCounter,
                                              fileMemoryImage->currentlyWordsInDataImage,
        /* PC + amount of words in CI <=>*/   (fileMemoryImage->currentlyWordsInCodeImage + fileMemoryImage->PC));
    validateLexTree(thisLexTree, openingLabelNDefinitionList);
    listsUpdating(openingLabelNDefinitionList, entryNExternalList, thisLexTree);
    codingThisLexTree(thisLexTree, openingLabelNDefinitionList, fileMemoryImage);
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


void codingThisLexTree(lexTree* thisLexTree,
                       labelOrDefinitionList *openingLabelNDefinitionList, memoryImage* fileMemoryImage){
    int amountOfWordsToCode = ZEROISE_COUNTER;
    char** wordsToCode;
    if(thisLexTree->error != valid)
        return;
    if(thisLexTree->type == order){
        wordsToCode = getBinaryRepresentationOfThisOrder(thisLexTree,&amountOfWordsToCode,
                                                         openingLabelNDefinitionList);
            addToCodeImage(fileMemoryImage, wordsToCode, amountOfWordsToCode);
    }else if(thisLexTree->content.directionSentence.type == dataDirection) {
        wordsToCode = getBinaryRepresentationOfDirection(thisLexTree, &amountOfWordsToCode,
                                                         openingLabelNDefinitionList);
        addToDataImage(fileMemoryImage, wordsToCode, getArgumentAmountInDataContent(thisLexTree));
    }else if (thisLexTree->content.directionSentence.type == stringDirection){
        wordsToCode = getBinaryRepresentationOfDirection(thisLexTree, &amountOfWordsToCode,
                                                         openingLabelNDefinitionList);
        addToDataImage(fileMemoryImage, wordsToCode, amountOfWordsToCode);
    }
}



void dataImageEndOfFirstPassUpdating(memoryImage *fileMemoryImage,
                                     labelOrDefinitionList* openingLabelNDefinitionList){
    labelNode *current = openingLabelNDefinitionList->head;
    while(current->next != NULL){
        if(current->labelType == data){
            resetPC(current, 100 + fileMemoryImage->currentlyWordsInCodeImage);
        }
        current = (labelNode *) current->next;
    }
    if(current->labelType == data){
        resetPC(current, 100 + fileMemoryImage->currentlyWordsInCodeImage);
    }
}



/*
labelNode *current = openingLabelNDefinitionList->head;
while(current->next != NULL){
    printf("%s  \n",current->title);
    current = (labelNode *) current->next;
}
printf("%s  \n",current->title);
*/











