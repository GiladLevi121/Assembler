
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
    data,
    code,
    mDefine
}labelCharacteristics;

typedef struct /*labelNode*/ {
    errorType labelError;
    char title[MAX_CHARS_IN_LINE];
    labelCharacteristics labelType;
    union {
        char PC;
        char definitionValue[MAX_NUMBER_LENGTH_IN_DEFINE];
    }value;
    struct labelOrDefinitionNode *next;
} labelNode;

/*------------------------------list------------------------------*/

typedef struct {
    labelNode * head;
}labelOrDefinitionList;

/*------------------------------node functions------------------------------*/

/*this function gets an assembly line code and returns opening label if existed,
 *by searching ':'. returns NULL if doesn't fond*/
char *extractOpeningLabelTitle(const assemblyLineCode *assemblyLine);

/*construct and set label node*/
labelNode *labelNodeConstructor(const assemblyLineCode*);

/*sets title*/
void setLabelTitle(labelNode *, const assemblyLineCode* );

/*return label length with ":"*/
size_t getLabelLengthWithLabelIdentifier(const labelNode*);

/*Construct labelNode based on definition sentence*/
labelNode* labelDefinitionNodeConstructor(const char *, const char *);
/*------------------------------list functions------------------------------*/

labelOrDefinitionList* labelOrDefinitionListConstructor();

void addLabelOrDefinitionNodeAtTheEnd(labelOrDefinitionList *, labelNode *);

boolean isLabelAppearInList(labelNode*, labelOrDefinitionList*);

void deallocateLabelListElements(labelOrDefinitionList *);

/*return true if names are equal, newNode.errorType = labelTitleAlreadyUsed*/
boolean areEqualNames(labelNode* newNode, labelNode* );

/* Return definition value if found in list, null if didn't.
 * Searching by name. Assuming nodes.labelType already mDefine*/
char* getDefinitionValueFromList(labelOrDefinitionList *, const char*);

/* Same as "getDefinitionValueFromList" function only ignore beginning and closing white spaces*/
char* getDefinitionValueFromListIgnoreWhiteSpaces(labelOrDefinitionList *t, const char*);
#endif
