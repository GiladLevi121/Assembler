
#ifndef ASSEMBLER_SECONDPASSLEXTREEVALIDATION_H
#define ASSEMBLER_SECONDPASSLEXTREEVALIDATION_H

#include "lexTree.h"
#include "lexTreeValidation.h"
#include "globaldefinitionsNStructures.h"
#include "entryExternDeclaration.h"

/* Main function of validation gor second pass*/
void secondPassOrderValidation(lexTree *thisLexTree, labelOrDefinitionList *openingLabelNDefinitionList, entryExternList *entryExternLabelList);

/* Checks if label that used as direct address method is known, sets error if not*/
void setValidationStatusForDirectAddressMethodSecondPass(lexTree *thisLexTree, labelOrDefinitionList *openingLabelNDefinitionList, entryExternList *entryExternLabelList, const char *operand);

/* Setting error for lexTree in case .entry "someEntryDeclaration" isn't appear in openingLabelList
 * finction assuming this lexTree.type = direction*/
void setEntryErrorIfNeeded(lexTree *thisLexTree, labelOrDefinitionList*);

/* Checks if label that used as fixed index address method is known, sets error if not*/
void setValidationStatusForFixedIndexAddressMethod(lexTree* thisLexTree,
                                                   labelOrDefinitionList* openingLabelNDefinitionList,
                                                   entryExternList* entryExternLabelList,
                                                   const char* operand);
#endif
