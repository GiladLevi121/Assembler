
#ifndef ASSEMBLER_MACROUTILES_H
#define ASSEMBLER_MACROUTILES_H

#include "line.h"


extern const char* stringMacroErrors[];

typedef enum {
    validMacro = 0,
    macroNameAlreadyUsed,
    emptyNamingForMacro,
    generalErrorInMacroNaming,
    macroNameIsAReservedWord,
    errorInInnerLineOfTheMacroContent
}macroError;


typedef struct {
    macroError error;
    char* name;
    char* content;
    struct macroNode* next;
}macroNode;


typedef struct {
    macroNode * head;
}macroList;


/*------------------------------node functions------------------------------*/

/* Construct macro*/
macroNode * macroConstructor(const char*, const char*);

/* Freeing macro*/
void freeMacroNode(macroNode* );

/*------------------------------list functions------------------------------*/

/* Construct a list*/
macroList * macroListConstructor();

/* Add node to lise, sets error if macro.name already used*/
void addMacroToList(macroNode*, macroList*);

/* Return macro content according to the macro name entered, return NULL
 * if macro name didn't find in the list*/
char* getMacroContentByName(const char*, macroList*);

/* de-allocating the list*/
void deAllocateMacroList(macroList* thisList);
#endif
