
#include <stdlib.h>
#include <string.h>

#include "macroUtiles.h"

const char* stringMacroErrors[] = {
        "valid macro",
        "Macro name already used",
        "Empty naming for macro",
        "General error in macroNaming",
        "Macro name is a reserved word"
};

/*------------------------------node functions------------------------------*/

macroNode * macroConstructor(const char* macroName, const char* macroContent){
    macroNode * newMacro = (macroNode*) malloc(sizeof(macroNode));
    newMacro->error = validMacro;
    newMacro->name = (char*) malloc((strlen(macroName) + PADDING_CELL_LEN) * sizeof (char));
    newMacro->content = (char*) malloc((strlen(macroContent) + PADDING_CELL_LEN) * sizeof (char));
    strcpy(newMacro->name, macroName);
    strcpy(newMacro->content, macroContent);
    return newMacro;
}

void freeMacroNode(macroNode* macroToFree){
    if (macroToFree == NULL)
        return;
    if(macroToFree->name != NULL)
        free(macroToFree->name);
    if (macroToFree->content != NULL)
        free(macroToFree->content);
}


/*------------------------------list functions------------------------------*/

macroList * macroListConstructor(){
    macroList * newMacroList = (macroList*) malloc(sizeof(macroList));
    newMacroList->head = NULL;
    return newMacroList;
}

char* getMacroContentByName(const char* macroName, macroList* thisList){
    macroNode * current = thisList->head;
    if(thisList->head == NULL)
        return NULL;

    while (current->next != NULL){
        if (!strcmp(macroName, current->name))
            return current->content;
        current = (macroNode*) current->next;
    }
    return NULL;
}

void addMacroToList(macroNode* macroToAdd, macroList* thisList){
    macroNode* current = thisList->head;
    macroToAdd->next = NULL;
    macroToAdd->next = NULL;
    if(thisList->head == NULL){
        thisList->head = macroToAdd;
        return;
    }

    while (current->next != NULL){
        if (!strcmp(macroToAdd->name, current->name))
            macroToAdd->error = macroNameAlreadyUsed;
        current = (macroNode*) current->next;
    }
    current->next = (struct macroNode *) macroToAdd;
}

void deAllocateMacroList(macroList* thisList){
    macroNode* current = thisList->head;
    while (current != NULL){
        macroNode * temp = (macroNode*) current->next;
        freeMacroNode(current);
        current = temp;
    }
    free(thisList);
}

