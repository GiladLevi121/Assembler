
#ifndef ASSEMBLER_ENTRYEXTERNDECLARATION_H
#define ASSEMBLER_ENTRYEXTERNDECLARATION_H

#include "globaldefinitionsNStructures.h"

#define INITIATE_SIZE_FOR_USED_LINE_EXTERN_LABEL 32

/*------------------------------node------------------------------*/
typedef enum{
    externDeclaration,
    entryDeclaration
}nodeType;


typedef struct {
    int instructionCounter;
    errorType error;
    char* title;
    nodeType type;
    int amountOfLines;
    union {
        char** usedLines;
        char* declaredLine;
    }content;
    struct entryExternNode* next;
}entryExternNode;

/*------------------------------list------------------------------*/

typedef struct {
    entryExternNode * head;
}entryExternList;

/*------------------------------node functions------------------------------*/

/* Setting new node with entered title and instructionCounter. If title  isn't valid, return null. Also
 * sets the node type*/
entryExternNode * entryExternNodeConstructor(const char*, nodeType, int);

/* Construct the content*/
void constructNodeContent(entryExternNode *,nodeType);

/* Sets the declared line of entry label*/
void constructEntryDeclaredLine(entryExternNode *thisNode, const char *lineDeclared);

/* Sets another line used in extern label*/
void addLineToExternUsedLines(entryExternNode *, const char*);

/* Free each member and node*/
void freeEntryExternNode(entryExternNode *);

/* */
void constructExternUsedLines(entryExternNode* thisNode);

void constructEntryDeclaredLine(entryExternNode* , const char* );

/*------------------------------list functions------------------------------*/

/* Construct a list*/
entryExternList* entryExternListConstructor();

/* Return true if entryExternNode.title appears in list Also sets
 * error of name are equal*/
boolean setErrorIfEqualNodesNames(entryExternNode*, entryExternNode* );

/* Adding new node to the list, sets error if name already appears*/
void addNodeToEntryExternList(entryExternList* , entryExternNode*);

/* Freeing list*/
void deallocatingEntryExternList(entryExternList*);

/* Returns true if title appears*/
boolean isTileAppearInEntryExternAsExternDeclarationList(const char *, entryExternList *);

/* Return node with this title if appears, null if not*/
entryExternNode * nodeWithThisTitle(const char *, entryExternList *);
#endif
