
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

/* Check for second pass errors*/
void additionValidationSecondPass(lexTree *,labelOrDefinitionList *,entryExternList*);

/* Main function for second pass coding */
void secondPassCoding(lexTree *, labelOrDefinitionList *, entryExternList *, memoryImage*);

/* Creates the output files*/
void createOutputFiles(memoryImage *, entryExternList *, const char*);

/* Creates the output externNEntryFile*/
void createEntNExtFile(entryExternList *, const char*);

/* Creates the output obj file*/
void createObjFile(memoryImage*, const char*);
#endif
