#include <stdlib.h>
#include <string.h>

#include "lexTree.h"

void initDataDeclarationDoublePointer(lexTree* thisLexTree, size_t pointersAmount){
    int counter = ZEROISE_COUNTER;
    if(pointersAmount <= ZERO_TOKENS){
        thisLexTree->content.directionSentence.content.dataDirection = NULL;
        return;
    }
    else
        thisLexTree->content.directionSentence.content.dataDirection = (char **) malloc(
                (pointersAmount + ANOTHER_CELL) * sizeof(char *));

    while(counter < pointersAmount + ANOTHER_CELL){
        thisLexTree->content.directionSentence.content.dataDirection[counter] = NULL;
        counter++;
    }
}

void addTokenToDataDirection(lexTree* thisLexTree, char* token){
    int i = ZEROISE_COUNTER;
    while(thisLexTree->content.directionSentence.content.dataDirection[i] != NULL){
        i++;
    }
    thisLexTree->content.directionSentence.content.dataDirection[i] =
            (char *) malloc(strlen(token) * sizeof (char));
    thisLexTree->content.directionSentence.content.dataDirection[i] = token;
}

void initNSetEntryDeclaration(lexTree* thisLexTree, const char* entryLabel){
    if(entryLabel == NULL){
        thisLexTree->content.directionSentence.content.entryLabel = NULL;
    } else{
        thisLexTree->content.directionSentence.content.entryLabel = (char*) malloc(
                strlen(entryLabel) * sizeof(char));
        strcpy(thisLexTree->content.directionSentence.content.entryLabel, entryLabel);
    }
}

void initNSetExternDeclaration(lexTree* thisLexTree, const char* externLabel){
    if(externLabel == NULL){
        thisLexTree->content.directionSentence.content.externLabel = NULL;
    } else{
        thisLexTree->content.directionSentence.content.externLabel = (char*) malloc(
                strlen(externLabel) * sizeof(char));
        strcpy(thisLexTree->content.directionSentence.content.externLabel, externLabel);
    }
}

void initNSetStringDeclaration(lexTree* thisLexTree, const char* assemblyStr){
    if(assemblyStr == NULL){
        thisLexTree->content.directionSentence.content.stringContent = NULL;
    } else{
        thisLexTree->content.directionSentence.content.stringContent = (char*) malloc(
                strlen(assemblyStr) * sizeof(char));
        strcpy(thisLexTree->content.directionSentence.content.stringContent, assemblyStr);
    }
}

void initNSetDestinationOperand(lexTree* thisLexTree, const char* destinationOperand){
    if(destinationOperand == NULL){
        thisLexTree->content.orderContent.destinationOperand = NULL;
    } else{
        thisLexTree->content.orderContent.destinationOperand = (char*) malloc(
                strlen(destinationOperand) * sizeof(char));
        strcpy(thisLexTree->content.orderContent.destinationOperand, destinationOperand);
    }
}

void initNSetSourceOperand(lexTree* thisLexTree, const char* sourceOperand){
    if(sourceOperand == NULL)
        thisLexTree->content.orderContent.sourceOperand = NULL;
    else{
        thisLexTree->content.orderContent.sourceOperand = (char*) malloc(
                strlen(sourceOperand) * sizeof(char));
        strcpy(thisLexTree->content.orderContent.sourceOperand, sourceOperand);
    }
}

void initNSetDefinitionName(lexTree* thisLexTree, int definitionNameLength,
                            const char* definitionName){
    if(definitionName == NULL)
        thisLexTree->content.definitionContent.name = NULL;
    else{
        thisLexTree->content.definitionContent.name = (char*) malloc(definitionNameLength * sizeof(char));
        memcpy(thisLexTree->content.definitionContent.name, definitionName, definitionNameLength);
        thisLexTree->content.definitionContent.name[definitionNameLength] = END_OF_STRING;
    }
}

void initNSetDefinitionValue(lexTree* thisLexTree, int definitionValueLength,
                             const char* definitionValue){
    if(definitionValue == NULL)
        thisLexTree->content.definitionContent.value = NULL;
    else{
        thisLexTree->content.definitionContent.value = (char*)malloc(definitionValueLength * sizeof (char));
        memcpy(thisLexTree->content.definitionContent.value,
               definitionValue,/*definitionValueStartingIndex*/
               definitionValueLength);
        thisLexTree->content.definitionContent.value[definitionValueLength] = END_OF_STRING;
    }
}

void freeLexTree(lexTree* thisLexTree) {
    if (thisLexTree->type == definition){
        if (thisLexTree->content.definitionContent.value != NULL)
            free(thisLexTree->content.definitionContent.value);
        if (thisLexTree->content.definitionContent.name != NULL)
            free(thisLexTree->content.definitionContent.name);
    }else if(thisLexTree->type == order){
        if(thisLexTree->content.orderContent.destinationOperand != NULL)
            free(thisLexTree->content.orderContent.destinationOperand);
        if (thisLexTree->content.orderContent.sourceOperand != NULL)
            free(thisLexTree->content.orderContent.sourceOperand);
    } else /* thisLexTreeType = directionSentence*/{
        if (thisLexTree->content.directionSentence.type == stringDirection &&
        thisLexTree->content.directionSentence.content.stringContent != NULL)
                free(thisLexTree->content.directionSentence.content.stringContent);
        if (thisLexTree->content.directionSentence.type == entryDirection &&
                thisLexTree->content.directionSentence.content.entryLabel != NULL)
            free(thisLexTree->content.directionSentence.content.entryLabel);
        if (thisLexTree->content.directionSentence.type == externDirection &&
                thisLexTree->content.directionSentence.content.externLabel != NULL)
            free(thisLexTree->content.directionSentence.content.externLabel);
        if(thisLexTree->content.directionSentence.type == dataDirection) {
            freeDataDirectionContent(thisLexTree);
        }
    }
    free(thisLexTree);
}

void freeDataDirectionContent(lexTree* thisLexTree){
    int i = ZEROISE_COUNTER;
    for (; thisLexTree->content.directionSentence.content.dataDirection[i] != NULL; i++) {
        free(thisLexTree->content.directionSentence.content.dataDirection[i]);
    }
    free(thisLexTree->content.directionSentence.content.dataDirection);
}

void resetInnerIndex(lexTree* thisLexTree, size_t plusIndex){
    thisLexTree->rawLineInnerIndex = thisLexTree->rawLineInnerIndex + plusIndex;
}

size_t getArgumentAmountInDataContent(lexTree* thisLexTree){
    DirectionSentence thisDirection= thisLexTree->content.directionSentence;
    size_t i = ZEROISE_COUNTER;
    while(thisDirection.content.dataDirection[i] != NULL){
        i++;
    }
    return i;
}











