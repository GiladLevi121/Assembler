

#ifndef ASSEMBLER_LEXTREEVALIDATION_H
#define ASSEMBLER_LEXTREEVALIDATION_H

#include "lexTree.h"

typedef enum{
    immediate = 0,
    direct = 1,
    fixedIndex = 2,
    directRegister = 3
}addressMethod;

void validateLexTree(lexTree *);

/*------------------------------definition validation functions------------------------------*/

void validateDefinitionLexTree(lexTree *);



/*------------------------------order validation functions------------------------------*/

void validateOrderLexTree(lexTree *);

/*gets operand as string and return addressMethod*/
addressMethod determineAddressMethod(const char *);

/* Return true if the rest of the operand is known label
 * or legal 12 bits number without white spaces.
 * Assuming '#' is in front of rest of the operand.*/
boolean isKnownLabelOrLegalNumber(const char* /*, labelOrDefinitionList**/);

/*------------------------------direction validation functions------------------------------*/

void validateDirectionLexTree(lexTree *);

#endif
