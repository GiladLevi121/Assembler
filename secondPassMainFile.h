
#ifndef ASSEMBLER_SECONDPASSMAINFILE_H
#define ASSEMBLER_SECONDPASSMAINFILE_H

#include "memoryImage.h"
#include "globaldefinitionsNStructures.h"
#include "label.h"
#include "filesUtil.h"
#include "lexTree.h"
#include "entryExternDeclaration.h"

/* Main function of second pass*/
void runSecondPass(char *, /*boolean errorExist,*/
                   labelOrDefinitionList* ,
                   entryExternList * , memoryImage* );

/* Main operations that every line of assembly code need to be done,
 * in the second pass*/
void secondPassEveryLineOfAssemblyOperations(assemblyLineCode *, int, labelOrDefinitionList*,
                                             entryExternList *, memoryImage*);

void additionValidationSecondPass(lexTree *,labelOrDefinitionList *,entryExternList*);
#endif
