

#ifndef ASSEMBLER_LEXTREEVALIDATION_H
#define ASSEMBLER_LEXTREEVALIDATION_H

#include "lexTree.h"

void validateLexTree(lexTree *);

/*------------------------------definition validation functions------------------------------*/

void validateDefinitionLexTree(lexTree *);

void checkForForbiddenOpeningLabel(lexTree*);


/*------------------------------order validation functions------------------------------*/

void validateOrderLexTree(lexTree *);

/*------------------------------direction validation functions------------------------------*/

void validateDirectionLexTree(lexTree *);

#endif
