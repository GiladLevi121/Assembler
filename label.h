
#ifndef ASSEMBLER_LABEL_H
#define ASSEMBLER_LABEL_H

#include <stdio.h>

#include "globaldefinitionsNStructures.h"
#include "line.h"




#define LABEL_MAX_LENGTH 31
#define LABEL_MAX_LENGTH_WITH_PADDING_CELL 32
#define NO_LABEL '\0'
#define LABEL_IDENTIFIER ':'
#define LABEL_IDENTIFIER_LENGTH 1

/*------------------------------structs------------------------------*/

/*------------------------------Node------------------------------*/
typedef enum{
    codeSymbol,
    dataSymbol,
    definitionSymbol,
}propertyType;

typedef struct /*labelNode*/ {
    int PC;
    errorType labelError;
    char title[MAX_CHARS_IN_LINE];
    propertyType thisLabelType;
    union {
        char value[MEMORY_WORD_LENGTH];
        char codeImag[CHARS_TO_REPRESENT_LINE_IN_MEMORY];
        char dataImag[CHARS_TO_REPRESENT_LINE_IN_MEMORY];
    }content;
    struct labelOrDefinitionNode *next;
} labelOrDefinitionNode;

/*------------------------------list------------------------------*/

typedef struct {
    labelOrDefinitionNode * head;
}labelOrDefinitionList;

/*------------------------------node functions------------------------------*/

/*this function gets an assembly line code and returns opening label if existed,
 *by searching ':'. returns NULL if doesn't fond*/
char *extractOpeningLabelTitle(const assemblyLineCode *assemblyLine);

/*construct and set label node*/
labelOrDefinitionNode *labelNodeConstructor(const assemblyLineCode*);

/*sets title*/
void setLabelTitle(labelOrDefinitionNode *, const assemblyLineCode* );

/*return label length with ":"*/
size_t getLabelLengthWithLabelIdentifier(const labelOrDefinitionNode*);


labelOrDefinitionNode* definitionNodeConstructor(const char *, const char *);
/*------------------------------list functions------------------------------*/

labelOrDefinitionList* labelOrDefinitionListConstructor();

void addLabelOrDefinitionNodeAtTheEnd(labelOrDefinitionList *, labelOrDefinitionNode *);

/*linked list: labels, definitions, macros*/

void deallocateLabelListElements(labelOrDefinitionList *);

/*return true if names are equal, newNode.errorType = labelTitleAlreadyUsed*/
boolean areEqualNames(labelOrDefinitionNode* newNode, labelOrDefinitionNode* );

#endif
