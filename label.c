
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
    for (;(counter < lineLength) && (counter < LABEL_MAX_LENGTH_WITH_PADDING_CELL) && (assemblyLine->content[counter] != ':'); ++counter)
        potentialLabel[counter] = assemblyLine->content[counter];
    potentialLabel[counter] = END_OF_STRING;
    if (assemblyLine->content[counter] == LABEL_IDENTIFIER)
        return potentialLabel;
    else{
        free(potentialLabel);
        return NULL;
    }
}

labelNode *labelNodeConstructor(const assemblyLineCode* thisAssemblyLineCode){
    labelNode *newLabel = (labelNode *) malloc(sizeof(labelNode));
    newLabel->title = extractOpeningLabelTitle(thisAssemblyLineCode);
    if (newLabel->title == NULL){
        free(newLabel);
        return NULL;
    }
    newLabel->labelError = isNotLegalTitle(newLabel->title);
    newLabel->next = NULL;
    return newLabel;
}

size_t getLabelLengthWithLabelIdentifier(const labelNode* thisLabelNode) {
    size_t labelFinalLength = ZEROISE_COUNTER;
    if(thisLabelNode != NULL)
        labelFinalLength = strlen(thisLabelNode->title) + LABEL_IDENTIFIER_LENGTH;
    return labelFinalLength;
}


labelNode* labelDefinitionNodeConstructor(const char *enteredTitle, const char* enteredValue){
    labelNode *newDefinition = (labelNode *) malloc(sizeof(labelNode));
    newDefinition->title = (char*) malloc(strlen(enteredTitle) * sizeof (char));
    newDefinition->value.definitionValue = (char*) malloc(strlen(enteredValue) * sizeof (char ));
    strcpy(newDefinition->title, enteredTitle);
    strcpy(newDefinition->value.definitionValue, enteredValue);
    newDefinition->labelType = mDefine;
    return newDefinition;
}


/*------------------------------list functions------------------------------*/

labelOrDefinitionList * labelOrDefinitionListConstructor(){
    labelOrDefinitionList* newList = (labelOrDefinitionList * )malloc(sizeof (labelOrDefinitionList));
    newList->head = NULL;
    return newList;
}

void addLabelOrDefinitionNodeAtTheEnd(labelOrDefinitionList * thisList, labelNode *nodeToAdd){
    labelNode *current = thisList->head;
    nodeToAdd->next = NULL;
    if (thisList->head == NULL) {
        thisList->head = nodeToAdd;
        return;
    }
    while(current->next != NULL){
        areEqualNames(nodeToAdd, current);
        current = (labelNode *) current->next;
    }
    current->next = (struct labelOrDefinitionNode *) nodeToAdd;
}

boolean areEqualNames(labelNode* newNode, labelNode* otherNode){
    if (!strcmp(newNode->title, otherNode->title)) {
        newNode->labelError = labelTitleAlreadyUsed;
        return true;
    }
    return false;
}

void deallocateLabelListElements(labelOrDefinitionList *thisList) {
    labelNode *currentHead = thisList->head;
    while (currentHead != NULL) {
        labelNode *temp = (labelNode *) currentHead->next;
        free(currentHead);
        currentHead = temp;
    }
}

char* getDefinitionValueFromList(labelOrDefinitionList *thisList, const char* defineName){
    labelNode *current = thisList->head;
    while(current->next != NULL){
        if (current->labelType == mDefine && !strcmp(current->title, defineName))
            return current->value.definitionValue;
        current = (labelNode *) current->next;
    }
    if (current->labelType == mDefine && !strcmp(current->title, defineName))
        return current->value.definitionValue;
    return NULL;
}

char* getDefinitionValueFromListIgnoreWhiteSpaces(labelOrDefinitionList *thisList, const char* defineNameWithWhiteSpaces){
    char * defineName = trimLeadingNEndingWhitespace(defineNameWithWhiteSpaces);
    char* value = getDefinitionValueFromList(thisList, defineName);
    free(defineName);
    return value;
}