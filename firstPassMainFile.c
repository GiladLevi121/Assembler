
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
    int i = 0;
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
    externEntryListUpdating(entryNExternalList, openingLabelNDefinitionList);
    dataImageEndOfFirstPassUpdating(fileMemoryImage, openingLabelNDefinitionList);

    for (; i < fileMemoryImage->currentlyWordsInCodeImage; i++) {
        if(fileMemoryImage->codeImage[i] != NULL)
            printf("%d) %s\n", i  ,fileMemoryImage->codeImage[i]);
        else
            printf("%d) NULL\n", i);
    }
    i = 0;
    for (; i < fileMemoryImage->currentlyWordsInDataImage; i++) {
        if(fileMemoryImage->dataImage[i] != NULL)
            printf("%d) %s\n", i + (int)fileMemoryImage->currentlyWordsInCodeImage ,fileMemoryImage->dataImage[i]);
        else
            printf("%d) NULL\n", i + (int)fileMemoryImage->currentlyWordsInCodeImage);
    }
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
        addNodeToEntryExternList(entryNExternalList, newEntryExternNode);
    }
}


void addDefinitionToList(lexTree* thisLexTree, labelOrDefinitionList* labelNDefinitionList,
                         entryExternList* entryNExternalList){
    labelNode * newDefinitionNode = labelDefinitionNodeConstructor(
            thisLexTree->content.definitionContent.name,
            thisLexTree->content.definitionContent.value,
            thisLexTree->InstructionCounter);

    addLabelOrDefinitionNodeAtTheEnd(labelNDefinitionList, newDefinitionNode);
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
        if(current->type == entryDeclaration)
            entryListUpdating(current, openingLabelNDefinitionList);

        else/* (current->type == externDeclaration)*/
            setErrorIfExternDeclarationAppearAsInnerLabelOrDefinition(current, openingLabelNDefinitionList);

        current = (entryExternNode *)current->next;
    }

}

void entryListUpdating(entryExternNode * current, labelOrDefinitionList *openingLabelNDefinitionList){
    if (isTileAppearInLabelList(current->title, openingLabelNDefinitionList)){
        labelNode* pointer = getNodeIfAppearInLabelList(current->title, openingLabelNDefinitionList);
        if(pointer->labelType != mDefine) /* <=> code image or data image*/
            constructEntryDeclaredLine(current, pointer->value.PC);
    } else /* (!isTileAppearInLabelList(current->title, openingLabelNDefinitionList))*/
        current->error = declaredEntryLabelButDidntDeclaredLabelInFile;
}


void setErrorIfExternDeclarationAppearAsInnerLabelOrDefinition
(entryExternNode * current, labelOrDefinitionList *openingLabelNDefinitionList){
    if (isTileAppearInLabelList(current->title, openingLabelNDefinitionList)){
        labelNode* pointer = getNodeIfAppearInLabelList(current->title, openingLabelNDefinitionList);
        pointer->labelError = cantUseExternalDeclarationNameToAlreadyExistLabelOrDefinition;
        current->error = cantUseExternalDeclarationNameToAlreadyExistLabelOrDefinition;
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






