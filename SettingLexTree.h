

#ifndef ASSEMBLER_SETTINGLEXTREE_H
#define ASSEMBLER_SETTINGLEXTREE_H

#include <stdio.h>

#include "lexTree.h"
#include "label.h"

#define DEFINE_SENTENCE_IDENTIFIER ".define"
#define DEFINE_SENTENCE_IDENTIFIER_LENGTH 7

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

/*get how many arguments data direction has*/
size_t getDataArgumentAmount(lexTree* );

/*set content of string direction*/
void setStringDirectionContent(lexTree *);

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
