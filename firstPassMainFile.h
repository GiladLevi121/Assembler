

#ifndef ASSEMBLER_FIRSTPASSMAINFILE_H
#define ASSEMBLER_FIRSTPASSMAINFILE_H

#include "label.h"
#include "SettingLexTree.h"
#include "memoryImage.h"
#include "entryExternDeclaration.h"

/*this function activates first pass on a file. it gets
 *file's name (without ending),  */
void runFirstPass(char *, labelOrDefinitionList*, entryExternList*, memoryImage* );

/* Updating linked lists*/
void listsUpdating(labelOrDefinitionList*, entryExternList*, lexTree*);

/* Main operations that every line of assembly code need to be done*/
void firstPassEveryLineOfAssemblyOperations(assemblyLineCode *, int, labelOrDefinitionList*,
                                            entryExternList*, memoryImage*);

/* Send to coding according to the type*/
void codingThisLexTree(lexTree*, labelOrDefinitionList*, memoryImage*);

/* Add offset of ci to di*/
void dataImageEndOfFirstPassUpdating(memoryImage*, labelOrDefinitionList*);

#endif
