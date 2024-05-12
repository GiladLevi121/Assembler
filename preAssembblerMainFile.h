
#ifndef ASSEMBLER_PREASSEMBBLERMAINFILE_H
#define ASSEMBLER_PREASSEMBBLERMAINFILE_H

#include "line.h"

/* PreAssembler main function.*/
void runPreAssembler(char *);

void setAssemblyLineOfCodeStatusInPreAssemblerPass(assemblyLineCode* assemblyLine);

/* Return true if empty line*/
boolean isEmptyLine(assemblyLineCode *);

/* Return true if first char is ';', if only white spaces before also return true*/
boolean isCommentLine(assemblyLineCode *);

#endif
