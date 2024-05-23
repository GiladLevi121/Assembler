
#include <stdlib.h>
#include <stdio.h>

#include "firstPassMainFile.h"
#include "filesUtil.h"
#include "globaldefinitionsNStructures.h"
#include "line.h"
#include "SettingLexTree.h"
#include "lexTreeValidation.h"
#include "firstPassCoding.h"

void runFirstPass(char *fileName,
                  labelOrDefinitionList* openingLabelNDefinitionList,
                  entryExternList * entryNExternalList,
                  memoryImage* fileMemoryImage,
                  macroList* thisMacroList, boolean* fileErrorDetected, boolean* memoryFull){
    FILE *filePointer = openFileByName(fileName, AM_ENDING, "r");
    assemblyLineCode *newAssemblyLine;
    int instructionCounter = ZEROISE_COUNTER;
    while ((newAssemblyLine = getNextAssemblyLine(filePointer)) != NULL){
        firstPassEveryLineOfAssemblyOperations(newAssemblyLine, ++instructionCounter,
                                               openingLabelNDefinitionList, entryNExternalList,
                                               fileMemoryImage, thisMacroList, fileErrorDetected,
                                               fileName);
        if (isMemoryFull(fileMemoryImage)){
            char* outputFile = connect2strings(fileName, AM_ENDING);
            printf("Error in file: %s. Error details: Memory is out of bounds! The file is too big."
                   "Program already printed errors up to this point of assembly code.\n", outputFile);
            *memoryFull = true;
            free(outputFile);
        }
    }
    externEntryListUpdating(entryNExternalList, openingLabelNDefinitionList,
                            fileMemoryImage->currentlyWordsInCodeImage);
    dataImageEndOfFirstPassUpdating(fileMemoryImage, openingLabelNDefinitionList);
    fclose(filePointer);
}


void firstPassEveryLineOfAssemblyOperations(assemblyLineCode *newAssemblyLine, int instructionCounter,
                         labelOrDefinitionList* openingLabelNDefinitionList,
                         entryExternList* entryNExternalList, memoryImage* fileMemoryImage,
                         macroList* thisMacroList, boolean* fileErrorDetected, const char* fileName){

    lexTree *thisLexTree = lexTreeConstructor(newAssemblyLine, instructionCounter,
                                              fileMemoryImage->currentlyWordsInDataImage,
        /* PC + amount of words in CI <=>*/   (fileMemoryImage->currentlyWordsInCodeImage + fileMemoryImage->PC));

    validateLexTree(thisLexTree, openingLabelNDefinitionList);
    listsUpdating(openingLabelNDefinitionList, entryNExternalList,
                  thisLexTree, thisMacroList);
    if (thisLexTree->error == valid && *fileErrorDetected == false){
        codingThisLexTree(thisLexTree, openingLabelNDefinitionList, fileMemoryImage);
    }
    else{
        if (thisLexTree->error != valid){
        char* outputFile = connect2strings(fileName, AM_ENDING);
        *fileErrorDetected = true;
        printErrorToUser(outputFile, thisLexTree->error, thisLexTree->InstructionCounter);
        free(outputFile);
        }
    }
    free(newAssemblyLine);
    freeLexTree(thisLexTree);
}

void listsUpdating(labelOrDefinitionList* labelNDefinitionList,
                   entryExternList* entryNExternalList, lexTree* thisLexTree, macroList* thisMacrosList){
    if(thisLexTree->error != valid)
        return;
    if(thisLexTree->potentialLabel != NULL){
        addLabelOrDefinitionNodeAtTheEnd(labelNDefinitionList, thisLexTree->potentialLabel, thisMacrosList);
        if (thisLexTree->potentialLabel->labelError != valid)
            thisLexTree->error = thisLexTree->potentialLabel->labelError;
    }
    if (thisLexTree->type == definition){
        addDefinitionToList(thisLexTree, labelNDefinitionList, thisMacrosList);
        return;
    }
    if(thisLexTree->type == direction){
        addEntryOrExternToList(thisLexTree, entryNExternalList);
    }
}


void addEntryOrExternToList(lexTree* thisLexTree, entryExternList * entryNExternalList){
    if(thisLexTree->content.directionSentence.type == entryDirection){
        entryExternNode * newEntryExternNode = entryExternNodeConstructor(
                thisLexTree->content.directionSentence.content.entryLabel, entryDeclaration,
                thisLexTree->InstructionCounter);
        addNodeToEntryExternList(entryNExternalList, newEntryExternNode);
        if (newEntryExternNode->error != valid)
            thisLexTree->error = newEntryExternNode->error;
    }else if(thisLexTree->content.directionSentence.type == externDirection){
        entryExternNode * newEntryExternNode = entryExternNodeConstructor(
                thisLexTree->content.directionSentence.content.externLabel, externDeclaration,
                thisLexTree->InstructionCounter);
        constructExternUsedLines(newEntryExternNode);
        addNodeToEntryExternList(entryNExternalList, newEntryExternNode);
        if (newEntryExternNode->error != valid)
            thisLexTree->error = newEntryExternNode->error;
    }
}


void addDefinitionToList(lexTree* thisLexTree, labelOrDefinitionList* labelNDefinitionList,
                         macroList* thisMacroList){
    labelNode * newDefinitionNode = labelDefinitionNodeConstructor(
            thisLexTree->content.definitionContent.name,
            thisLexTree->content.definitionContent.value,
            thisLexTree->InstructionCounter);
    addLabelOrDefinitionNodeAtTheEnd(labelNDefinitionList, newDefinitionNode, thisMacroList);
    if (newDefinitionNode->labelError != valid){
        thisLexTree->error = newDefinitionNode->labelError;
    }
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
    if (openingLabelNDefinitionList->head == NULL)
        return;
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
                             labelOrDefinitionList *openingLabelNDefinitionList,
                             size_t wordsInCodeImage){
    entryExternNode * current = entryNExternalList->head;
    while (current != NULL){
        if(current->type == entryDeclaration)
            entryListUpdating(current, openingLabelNDefinitionList, wordsInCodeImage);

        else/* (current->type == externDeclaration)*/
            setErrorIfExternDeclarationAppearAsInnerLabelOrDefinition(current, openingLabelNDefinitionList);

        current = (entryExternNode *)current->next;
    }

}

void entryListUpdating(entryExternNode * current,
                       labelOrDefinitionList *openingLabelNDefinitionList, size_t wordsInCodeImage){
    if (isTileAppearInLabelList(current->title, openingLabelNDefinitionList)){
        labelNode* pointer = getNodeIfAppearInLabelList(current->title, openingLabelNDefinitionList);
        if(pointer->labelType == code) /* <=> code image or data image*/
            constructEntryDeclaredLine(current, pointer->value.PC);
        else if (pointer->labelType == data){
            int PCAsInt;
            char finalPcValue[CHARS_NEEDED_TO_REPRESENT_LAST_MEMORY_LINE];
            char* endPointer;
            PCAsInt = (int)(strtol(pointer->value.PC, &endPointer, DECIMAL) + wordsInCodeImage + FIRST_FREE_WORD);
            sprintf(finalPcValue, "%d", PCAsInt);
            constructEntryDeclaredLine(current, finalPcValue);
        }
        else /*pointer->labelType == mDefine*/{
            current->error = cantUseEntryDeclarationToReferToDefinitionName;
            pointer->labelError = cantUseEntryDeclarationToReferToDefinitionName;
        }
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



