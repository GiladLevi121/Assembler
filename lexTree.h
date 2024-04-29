

#ifndef ASSEMBLER_LEXTREE_H
#define ASSEMBLER_LEXTREE_H

#include <stdio.h>

#include "label.h"

#define DEFINE_SENTENCE_IDENTIFIER ".define"
#define DEFINE_SENTENCE_IDENTIFIER_LENGTH 7


/*------------------------------structs------------------------------*/

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
    char sourceOperand[MAX_CHARS_IN_LINE];
    char destinationOperand[MAX_CHARS_IN_LINE];
    commandOpcode opcode;
    opcodeGroup group;
}OrderSentence;

/*------------------------------definition sentence------------------------------*/

typedef struct {
    char name[LABEL_MAX_LENGTH_WITH_PADDING_CELL];
    char value[MAX_NUMBER_LENGTH_IN_DEFINE];
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
        char stringContent[MAX_STRING_LENGTH];
        char dataDirection[MAX_NUMBERS_IN_DATA_DECLARATION][MAX_NUMBER_LENGTH];
        char entryLabel[MAX_LABEL_LENGTH_AFTER_ENTRY];
        char externLabel[MAX_LABEL_LENGTH_AFTER_EXTERN];
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
    /*union {
        char codeImag[ORDER_MAX_WORD_IN_MEMORY][MEMORY_WORD_LENGTH];
        char dataImag[MAX_CHARS_IN_LINE][MEMORY_WORD_LENGTH];
        boolean nonCoding;
    }coding;*/
} lexTree;


/*------------------------------lexTree functions structure------------------------------*/
/*

                                        lexTreeConstructor
                                                |
                                                |
                                        labelNodeConstructor,
                                        setLexTreeType,
                                 _______setLexTreeContent_________
                                /               |                 \
                               /                |                  \
                              /                 |                   \
          setDirectionLexTreeContent   setOrderLexTreeContent   setDefinitionLexTreeContent
                            |
                            |
                            |_______setDirectionSentenceType,
                                    directionDeclarationCheck,
                            ________setDirectionContent________________
                           /                   |                       \
                          /                    |                        \
                         /                     |                         \




*/
/*------------------------------lexTree functions ------------------------------*/
/*lex tree constructor*/
lexTree *lexTreeConstructor(const assemblyLineCode *, int);

/*sets lex tree sentence type*/
void setLexTreeType(lexTree*);

/*set current index to plus size_t*/
void resetInnerIndex(lexTree*, size_t);

/*set lex tree content*/
void setLexTreeContent(lexTree *);

/*set error if find comma error*/
void commasValidation(lexTree *);

/*set lex tree if direction sentence*/
void setDirectionLexTreeContent(lexTree *);


/*------------------------------directions functions------------------------------*/

/*sets direction sentence type*/
void setDirectionSentenceType(lexTree* );

/*sets direction sentence content*/
void setDirectionContent(lexTree *);

/*check if mandatory space after direction declaration, and
 * doesn't end with a comma*/
void directionDeclarationCheck(lexTree *);

/*set content of data direction*/
void setDataDirectionContent(lexTree*);

/*set content of string direction*/
void setStringDirectionContent(lexTree *);

void assemblyStringValidation(lexTree *newLexTree, size_t, size_t);

/*for ".entry thisLabel" set thisLabel*/
void setEntryNExternContent(lexTree*);

/*------------------------------orders functions------------------------------*/
/*set order sentence content*/
void setOrderLexTreeContent(lexTree *);

void setOpCode(lexTree*);

void setGroup(lexTree*);

void setOperands(lexTree*);

void setFirstGroup(lexTree*);

void setSecondGroup(lexTree*);

void setThirdGroup(lexTree *);

void setUndefinedGroup(lexTree*);
/*------------------------------defines functions------------------------------*/
/*set definition sentence content*/
void setDefinitionLexTreeContent(lexTree*);

void setDefinitionName(lexTree*);

void setDefinitionValue(lexTree*);

#endif
