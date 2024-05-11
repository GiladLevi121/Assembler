
#ifndef ASSEMBLER_SECONDPASSLEXTREEVALIDATION_H
#define ASSEMBLER_SECONDPASSLEXTREEVALIDATION_H

#include "lexTree.h"
#include "lexTreeValidation.h"
#include "globaldefinitionsNStructures.h"
#include "entryExternDeclaration.h"

/* Main function of validation gor second pass*/
void secondPassMainFunctionValidation(lexTree*, labelOrDefinitionList*, entryExternList*);

/* Checks if label that used in order sentence is known, sets error if not*/
void setValidationStatusForDirectAddressMethod(lexTree*, labelOrDefinitionList*, entryExternList*, const char*);
#endif
