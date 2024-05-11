
#include <stdlib.h>
#include "stdio.h"

#include "entryExternDeclaration.h"
#include "labelNDefineValidation.h"


/*------------------------------node------------------------------*/

entryExternNode * entryExternNodeConstructor(const char* title, nodeType thisNodeType){
    entryExternNode* newNode = (entryExternNode*) malloc(sizeof(entryExternNode));
    newNode->error = isNotLegalTitle(title);
    newNode->type = thisNodeType;
    newNode->amountOfLines = ZEROISE_COUNTER;
    if(newNode->error == valid){
        newNode->title = (char*) malloc((strlen(title) + ANOTHER_CELL) * sizeof(char));
        strcpy(newNode->title, title);
        return newNode;
    }else{
        free(newNode);
        return NULL;
    }
}

void freeEntryExternNode(entryExternNode * thisNodeToFree){
    if(thisNodeToFree == NULL)
        return;
    if(thisNodeToFree->title != NULL)
        free(thisNodeToFree->title);
    if(thisNodeToFree->type == entryDeclaration && thisNodeToFree->amountOfLines != EMPTY)
        free(thisNodeToFree->content.declaredLine);
    if (thisNodeToFree->type == externDeclaration ){
        int i = ZEROISE_COUNTER;
        for(; i < thisNodeToFree->amountOfLines; i++){
            free(thisNodeToFree->content.usedLines[i]);
        }
        free(thisNodeToFree->content.usedLines);
    }
    free(thisNodeToFree);
}

void addLineToExternUsedLines(entryExternNode * thisNode, const char* newLine){
    thisNode->content.usedLines[thisNode->amountOfLines] =
            (char*)malloc(strlen(newLine) * sizeof(char));
    strcpy(thisNode->content.usedLines[thisNode->amountOfLines], newLine);
    thisNode->amountOfLines++;
    if(thisNode->amountOfLines == INITIATE_SIZE_FOR_USED_LINE_EXTERN_LABEL){
        thisNode->content.usedLines = (char**) realloc(thisNode->content.usedLines,
                                                       MEMORY_INCREMENT_FACTOR * thisNode->amountOfLines *
                                                               sizeof(char *));
    }
}

void constructExternUsedLines(entryExternNode* thisNode){
    thisNode->content.usedLines = (char**) malloc(INITIATE_SIZE_FOR_USED_LINE_EXTERN_LABEL * sizeof(char*));
}

void constructEntryDeclaredLine(entryExternNode* thisNode, const char* lineDeclared){
    thisNode->content.declaredLine = (char*) malloc(strlen(lineDeclared) * sizeof(char));
    if (lineDeclared == NULL){
        free(thisNode->content.declaredLine);
        return;
    }
    strcpy(thisNode->content.declaredLine, lineDeclared);
    thisNode->amountOfLines ++;
}

/*------------------------------list------------------------------*/

entryExternList* entryExternListConstructor(){
    entryExternList * newList = (entryExternList*) malloc(sizeof(entryExternList));
    if (newList != NULL){
        newList->head = NULL;
    }
    return newList;
}


boolean setErrorIfEqualNodesNames(entryExternNode* newNode, entryExternNode* nodeToCmp){
    if (!strcmp(newNode->title, nodeToCmp->title)) {
        newNode->error = entryOrExternDeclarationCantAppearTwice;
        return true;
    }
    return false;
}


void addNodeToEntryExternList(entryExternList* thisList, entryExternNode* nodeToAdd){
    entryExternNode *current = thisList->head;
    nodeToAdd->next = NULL;
    if(thisList->head == NULL){
        nodeToAdd->next = NULL;
        thisList->head = /*(entryExternNode*) */nodeToAdd;
        return;
    }
    while (current->next != NULL){
        setErrorIfEqualNodesNames(nodeToAdd, current);
        current = (entryExternNode *) current->next;
    }
    current->next = (struct entryExternNode *)nodeToAdd;
}


void deallocatingEntryExternList(entryExternList* listToFree){
    entryExternNode *currentHead = listToFree->head;
    while (currentHead != NULL) {
        entryExternNode *temp = (entryExternNode *) currentHead->next;
        freeEntryExternNode(currentHead);
        currentHead = temp;
    }
}

boolean isTileAppearInEntryExternAsExternDeclarationList(const char *titleToSearch, entryExternList *thisList){
    entryExternNode *current = thisList->head;
    if(titleToSearch == NULL)
        return false;
    while (current != NULL) {
        if(!strcmp(current->title, titleToSearch) && current->type == externDeclaration)
            return true;
        current = (entryExternNode *) current->next;
    }
    return false;
}

entryExternNode * nodeWithThisTitle(const char *titleToSearch, entryExternList *thisList){
    entryExternNode *current = thisList->head;
    if(titleToSearch == NULL)
        return NULL;
    while (current != NULL) {
        if(!strcmp(current->title, titleToSearch) && current->type == externDeclaration)
            return current;
        current = (entryExternNode *) current->next;
    }
    return NULL;
}