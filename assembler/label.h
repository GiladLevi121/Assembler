
#ifndef ASSEMBLER_LABEL_H
#define ASSEMBLER_LABEL_H

#include <stdio.h>

#include "globaldefinitionsNStructures.h"
#include "line.h"
#include "preAssembblerMainFile.h"




#define LABEL_MAX_LENGTH_WITH_PADDING_CELL 32
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
    int instructionCounter;
    errorType labelError;
    char *title;
    labelCharacteristics labelType;
    union {
        char *PC;
        char *definitionValue;
    }value;
    struct labelOrDefinitionNode *next;
} labelNode;

/*------------------------------list------------------------------*/

typedef struct {
    labelNode * head;
}labelOrDefinitionList;

/*------------------------------node functions------------------------------*/

/*this function gets an assembly line code and allocate memory for label.title,
 * sets there the opening label if existed by searching ':'. Or
 * free the title if doesn't fond*/
char *extractOpeningLabelTitle(const assemblyLineCode *assemblyLine);

/*construct and set label node*/
labelNode *labelNodeConstructor(const assemblyLineCode*);

/*return label length with ":"*/
size_t getLabelLengthWithLabelIdentifier(const labelNode*);

/*Construct labelNode based on definition sentence*/
labelNode* labelDefinitionNodeConstructor(const char *, const char *, int);

/* Freeing label node, don't free .next*/
void freeLabel(labelNode*);

/* Setting pc if label found*/
void setLabelType(labelNode*, boolean, size_t, size_t, int);

/* Add the int to the current pc. Assuming label.type = data/code*/
void resetPC(labelNode*, int );
/*------------------------------list functions------------------------------*/

labelOrDefinitionList* labelOrDefinitionListConstructor();

/* Adding label to list, set error if needed*/
void addLabelOrDefinitionNodeAtTheEnd(labelOrDefinitionList *, labelNode *, macroList*);

/* Returns true if title appears*/
boolean isTileAppearInLabelList(const char *, labelOrDefinitionList *);


labelNode *getNodeIfAppearInLabelList(const char*, labelOrDefinitionList* );

/* Free list*/
void deallocateLabelListElements(labelOrDefinitionList *);

/* Return true if names are equal, newNode.errorType = labelTitleAlreadyUsed*/
boolean setErrorIfEqualNames(labelNode* newNode, labelNode *otherNode);

/* Return definition value if found in list, null if didn't.
 * Searching by name. Assuming nodes.labelType already mDefine*/
char* getDefinitionValueFromList(labelOrDefinitionList *, const char*);

/* Same as "getDefinitionValueFromList" function only ignore beginning and closing white spaces*/
char* getDefinitionValueFromListIgnoreWhiteSpaces(labelOrDefinitionList *t, const char*);
#endif
