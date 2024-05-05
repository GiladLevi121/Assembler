
#ifndef ASSEMBLER_LEXTREE_H
#define ASSEMBLER_LEXTREE_H

#include <stdio.h>
#include "label.h"

#define ZERO_TOKENS 0
/*------------------------------order sentence------------------------------*/

typedef enum  {
    mov = 0,
    cmp = 1,
    add = 2,
    sub = 3,
    lea = 4,
    not = 5,
    clr = 6,
    inc = 7,
    dec = 8,
    jmp = 9,
    bne = 10,
    red = 11,
    prn = 12,
    jsr = 13,
    rts = 14,
    hlt = 15,
    UnfamiliarCommand = 16
}commandOpcode;

typedef enum {
    undefinedGroup,
    firstGroup,
    secondGroup,
    thirdGroup
}opcodeGroup;

typedef struct {
    char *sourceOperand;
    char *destinationOperand;
    commandOpcode opcode;
    opcodeGroup group;
}OrderSentence;

/*------------------------------definition sentence------------------------------*/

typedef struct {
    char *name;
    char *value;
}DefinitionSentence;

/*------------------------------direction sentence------------------------------*/
typedef struct {
    /*order match directionsTypeIdentifier in lexTree.c*/
    enum {
        stringDirection,
        dataDirection,
        entryDirection,
        externDirection,
        unfamiliarDirection
    } type;
    union {
        char *stringContent;
        /*[MAX_NUMBERS_IN_DATA_DECLARATION][MAX_NUMBER_LENGTH];*/
        char **dataDirection;
        char *entryLabel;
        char *externLabel;
    } content;
} DirectionSentence;

/*------------------------------lexTree------------------------------*/

typedef enum {
    order,
    direction,
    definition
} lexTreeType;


typedef struct {
    size_t rawLineInnerIndex;
    const assemblyLineCode *rawLine;
    int InstructionCounter;
    errorType error;
    labelNode *potentialLabel;
    lexTreeType type;
    union {
        OrderSentence orderContent;
        DirectionSentence directionSentence;
        DefinitionSentence definitionContent;
    } content;
} lexTree;

/*------------------------------directions functions------------------------------*/
/* Allocating memory for lexTree...entryLabel, according to const char* .len,
 * and copy it */
void initNSetEntryDeclaration(lexTree*, const char* );

/* Allocating memory for lexTree...externLabel, according to const char* .len,
 * and copy it */
void initNSetExternDeclaration(lexTree*, const char* );

/* Allocating memory for lexTree...stringContent, according to const char* .len,
 * and copy it */
void initNSetStringDeclaration(lexTree*, const char* );

/* Allocating the double pointer memory for lexTree...dataContent, according to
 * the size_t argument entered. *dataContent[x] = NULL, also puts NULL in another
 * cell to indicate end of data argument;*/
void initDataDeclarationDoublePointer(lexTree* , size_t);

/* Adding the char* to the next open lexTree....*dataDirection[i]*/
void addTokenToDataDirection(lexTree*, char*);

/* Helper function to free dataDirection*/
void freeDataDirectionContent(lexTree* );
/*------------------------------orders functions------------------------------*/


/* Allocating memory for lexTree...destinationOperand, according to const char* .len,
 * and copy it */
void initNSetDestinationOperand(lexTree*, const char* );

/* Allocating memory for lexTree...sourceOperand, according to const char* .len,
 * and copy it */
void initNSetSourceOperand(lexTree*, const char*);

/*------------------------------defines functions------------------------------*/

/* Allocating memory for lexTree...value, according to definitionValueLength,
 * and copy definitionValue */
void initNSetDefinitionValue(lexTree* thisLexTree, int definitionValueLength,
                             const char* definitionValue);

/* Allocating memory for lexTree...name, according to definitionValueLength,
 * and copy definitionName */
void initNSetDefinitionName(lexTree*, int, const char* );

/* Free allocated memory*/
void freeLexTree(lexTree* );

/*set current index to plus size_t*/
void resetInnerIndex(lexTree*, size_t);
#endif
