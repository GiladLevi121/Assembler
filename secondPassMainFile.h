
#ifndef ASSEMBLER_SECONDPASSMAINFILE_H
#define ASSEMBLER_SECONDPASSMAINFILE_H

#include "memoryImage.h"
#include "globaldefinitionsNStructures.h"
#include "label.h"
#include "filesUtil.h"

/* Main function of second pass*/
void runSecondPass(memoryImage*, boolean ,labelOrDefinitionList*,
                   labelOrDefinitionList* , char *);

/* Main operations that every line of assembly code need to be done,
 * in the second pass*/
void secondPassEveryLineOfAssemblyOperations(assemblyLineCode *, int, labelOrDefinitionList*,
                                             labelOrDefinitionList*, memoryImage*);


#endif
