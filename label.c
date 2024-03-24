
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "label.h"
#include "globaldefinitionsNStructures.h"
#include "labelNDefineValidation.h"


/*------------------------------labelNode functions------------------------------*/

char *extractOpeningLabelTitle(const assemblyLineCode* assemblyLine){
    char *potentialLabel = (char*)malloc(sizeof(char) * LABEL_MAX_LENGTH_WITH_PADDING_CELL);
    int counter = ZEROISE_COUNTER;
    size_t lineLength = strlen(assemblyLine->content);
    for (; (counter < lineLength) && (assemblyLine->content[counter] != ':'); ++counter)
        potentialLabel[counter] = assemblyLine->content[counter];
    potentialLabel[counter] = END_OF_STRING;
    if (assemblyLine->content[counter] == LABEL_IDENTIFIER)
        return potentialLabel;
    return NULL;
}

void setLabelTitle(labelOrDefinitionNode * thisLabelNode, const assemblyLineCode* thisAssemblyLineCode) {
    char *thisTitle = extractOpeningLabelTitle(thisAssemblyLineCode);
    if (thisTitle == NULL) {
        thisLabelNode->title[FIRST_INDEX] = NO_LABEL;
        return;
    }
    strcpy(thisLabelNode->title, thisTitle);
    free(thisTitle);
}

labelOrDefinitionNode *labelNodeConstructor(const assemblyLineCode* thisAssemblyLineCode){
    labelOrDefinitionNode *newLabel = (labelOrDefinitionNode *) malloc(sizeof(labelOrDefinitionNode));
    setLabelTitle(newLabel, thisAssemblyLineCode);
    if (newLabel == NULL)
        printf("failed to construct labelNode");
    if(!newLabel->title[FIRST_INDEX]){
        free(newLabel);
        return NULL;
    }
    newLabel->labelError = isLegalTitle(newLabel->title);
    newLabel->next = NULL;
    return newLabel;
}

size_t getLabelLengthWithLabelIdentifier(const labelOrDefinitionNode* thisLabelNode) {
    size_t labelFinalLength = ZEROISE_COUNTER;
    if(thisLabelNode != NULL)
        labelFinalLength = strlen(thisLabelNode->title) + LABEL_IDENTIFIER_LENGTH;
    return labelFinalLength;
}


labelOrDefinitionNode* definitionNodeConstructor(const char *enteredTitle, const char* enteredValue){
    labelOrDefinitionNode *newDefinition = (labelOrDefinitionNode *) malloc(sizeof(labelOrDefinitionNode));
    strcpy(newDefinition->title, enteredTitle);
    strcpy(newDefinition->content.value, enteredValue);
    newDefinition->thisLabelType = definitionSymbol;
    return newDefinition;
}


/*------------------------------list functions------------------------------*/

labelOrDefinitionList * labelOrDefinitionListConstructor(){
    labelOrDefinitionList* newList = (labelOrDefinitionList * )malloc(sizeof (labelOrDefinitionList));
    newList->head = NULL;
    return newList;
}

void addLabelOrDefinitionNodeAtTheEnd(labelOrDefinitionList * thisList, labelOrDefinitionNode *nodeToAdd){
    labelOrDefinitionNode *current = thisList->head;
    nodeToAdd->next = NULL;
    if (thisList->head == NULL) {
        thisList->head = nodeToAdd;
        return;
    }
    while(current->next != NULL){
        areEqualNames(nodeToAdd, current);
        current = (labelOrDefinitionNode *) current->next;
    }
    current->next = (struct labelOrDefinitionNode *) nodeToAdd;
}

boolean areEqualNames(labelOrDefinitionNode* newNode, labelOrDefinitionNode* otherNode){
    if (!strcmp(newNode->title, otherNode->title)) {
        newNode->labelError = labelTitleAlreadyUsed;
        return true;
    }
    return false;
}



void deallocateLabelListElements(labelOrDefinitionList *thisList) {
    labelOrDefinitionNode *currentHead = thisList->head;
    while (currentHead != NULL) {
        labelOrDefinitionNode *temp = (labelOrDefinitionNode *) currentHead->next;
        free(currentHead);
        currentHead = temp;
    }
}