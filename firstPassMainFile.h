

#ifndef ASSEMBLER_FIRSTPASSMAINFILE_H
#define ASSEMBLER_FIRSTPASSMAINFILE_H

#include "label.h"
#include "SettingLexTree.h"

/*this function activates first pass on a file. it gets
 *file's name (without ending),  */
void runFirstPass(char *, labelOrDefinitionList*, labelOrDefinitionList* );

void listsUpdating(labelOrDefinitionList*, labelOrDefinitionList*, lexTree*);

/*main operations that every line of assembly code need to be done*/
void firstPassEveryLineOfAssemblyOperations(assemblyLineCode *, int ,labelOrDefinitionList* ,
                         labelOrDefinitionList*);

/*send to coding according to the type*/
void codingThisLexTree(lexTree* );

#endif
