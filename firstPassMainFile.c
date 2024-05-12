
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
#include "entryExternDeclaration.h"

void runFirstPass(char *fileName,
                  labelOrDefinitionList* openingLabelNDefinitionList,
                  entryExternList * entryNExternalList,
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
    externEntryListUpdating(entryNExternalList, openingLabelNDefinitionList);
    deallocatingEntryExternList(entryNExternalList);
    deallocateLabelListElements(openingLabelNDefinitionList);
    freeMemoryImage(fileMemoryImage);
    /*labelNode *current = openingLabelNDefinitionList->head;
    while(current->next != NULL){
        printf("%s  \n",current->title);
        current = (labelNode *) current->next;
    }
    printf("%s  \n",current->title);

    entryExternNode *temp = entryNExternalList->head;
    while(current->next != NULL){
        printf("%s  \n",temp->title);
        temp = (entryExternNode *) temp->next;
    }
    printf("%s  \n",temp->title);*/
    fclose(filePointer);
}


void firstPassEveryLineOfAssemblyOperations(assemblyLineCode *newAssemblyLine, int instructionCounter,
                         labelOrDefinitionList* openingLabelNDefinitionList,
                         entryExternList* entryNExternalList,
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
                   entryExternList* entryNExternalList, lexTree* thisLexTree){
    if(thisLexTree->error != valid)
        return;
    if(thisLexTree->potentialLabel != NULL){
        if(isTileAppearInEntryExternAsExternDeclarationList(
                thisLexTree->potentialLabel->title, entryNExternalList)){
            thisLexTree->error = cantUseEntryLabelOrDefinitionNameToAlreadyExistExternalDeclaration;
            thisLexTree->error = conflictNaming;
            entryExternNode* detectedRepeatedNodeTitle;
            detectedRepeatedNodeTitle = nodeWithThisTitle(thisLexTree->potentialLabel->title, entryNExternalList);
            detectedRepeatedNodeTitle->error = conflictNaming;
        }
        addLabelOrDefinitionNodeAtTheEnd(labelNDefinitionList, thisLexTree->potentialLabel);
    }
    if (thisLexTree->type == definition){
        addDefinitionToList(thisLexTree, labelNDefinitionList, entryNExternalList);
        return;
    }
    if(thisLexTree->type == direction){
        addEntryOrExternToList(thisLexTree, entryNExternalList, labelNDefinitionList);
    }
}


void addEntryOrExternToList(lexTree* thisLexTree, entryExternList * entryNExternalList,
                            labelOrDefinitionList* labelNDefinitionList){
    if(thisLexTree->content.directionSentence.type == entryDirection){
        entryExternNode * newEntryExternNode = entryExternNodeConstructor(
                thisLexTree->content.directionSentence.content.entryLabel, entryDeclaration);
        addNodeToEntryExternList(entryNExternalList, newEntryExternNode);
    }else if(thisLexTree->content.directionSentence.type == externDirection){
        entryExternNode * newEntryExternNode = entryExternNodeConstructor(
                thisLexTree->content.directionSentence.content.externLabel, externDeclaration);
        constructExternUsedLines(newEntryExternNode);
        if(isTileAppearInLabelList(newEntryExternNode->title, labelNDefinitionList)){
            thisLexTree->error = cantUseExternalDeclarationNameToAlreadyExistLabel;
            newEntryExternNode->error = cantUseExternalDeclarationNameToAlreadyExistLabel;
            labelNode * detectedRepeatedNodeTitle;
            detectedRepeatedNodeTitle = getNodeIfAppearInLabelList(newEntryExternNode->title, labelNDefinitionList);
            detectedRepeatedNodeTitle->labelError = conflictNaming;
        }
        addNodeToEntryExternList(entryNExternalList, newEntryExternNode);
    }
}


void addDefinitionToList(lexTree* thisLexTree, labelOrDefinitionList* labelNDefinitionList,
                         entryExternList* entryNExternalList){
    labelNode * newDefinitionNode = labelDefinitionNodeConstructor(
            thisLexTree->content.definitionContent.name,
            thisLexTree->content.definitionContent.value,
            thisLexTree->InstructionCounter);
    if(isTileAppearInEntryExternAsExternDeclarationList(
            newDefinitionNode->title, entryNExternalList)){
        newDefinitionNode->labelError = cantUseEntryLabelOrDefinitionNameToAlreadyExistExternalDeclaration;
        entryExternNode * detectedRepeatedNodeTitle;
        detectedRepeatedNodeTitle = nodeWithThisTitle(newDefinitionNode->title, entryNExternalList);
        detectedRepeatedNodeTitle->error = conflictNaming;
    }
    addLabelOrDefinitionNodeAtTheEnd(labelNDefinitionList, newDefinitionNode);
    if(newDefinitionNode->labelError == labelTitleAlreadyUsed)
        newDefinitionNode->labelError = definitionNamingAlreadyInUse;

    if (newDefinitionNode->labelError != valid)
        thisLexTree->error = newDefinitionNode->labelError;
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
            resetPC(current, FIRST_FREE_WORD + fileMemoryImage->currentlyWordsInCodeImage);
        }
        current = (labelNode *) current->next;
    }
    if(current->labelType == data){
        resetPC(current, FIRST_FREE_WORD + fileMemoryImage->currentlyWordsInCodeImage);
    }
}


void externEntryListUpdating(entryExternList *entryNExternalList,
                             labelOrDefinitionList *openingLabelNDefinitionList){
    entryExternNode * current = entryNExternalList->head;
    while (current != NULL){
        if (current->type == entryDeclaration &&
        isTileAppearInLabelList(current->title, openingLabelNDefinitionList)){
            labelNode* pointer = getNodeIfAppearInLabelList(current->title, openingLabelNDefinitionList);
            if(pointer->labelType != mDefine) /* <=> code image or data image*/
                constructEntryDeclaredLine(current, pointer->value.PC);
        } else if (!isTileAppearInLabelList(current->title, openingLabelNDefinitionList) &&
        current->type == entryDeclaration){
            current->error = declaredEntryLabelButDidntDeclaredLabelInFile;
        }
        current = (entryExternNode *)current->next;
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




/*labelNode *current = openingLabelNDefinitionList->head;
    while(current->next != NULL){
        if(current->labelType == data ||
        current->labelType == code){
            printf("%s  %s\n", current->title, current->value.PC);
        }else
            printf("%s  %s\n", current->title, current->value.definitionValue);
        current = (labelNode *) current->next;
    }
    if(current->labelType == data ||
       current->labelType == code){
        printf("%s  %s\n", current->title, current->value.PC);
    }else
        printf("%s  %s\n", current->title, current->value.definitionValue);*/






