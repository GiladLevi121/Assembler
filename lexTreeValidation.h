

#ifndef ASSEMBLER_LEXTREEVALIDATION_H
#define ASSEMBLER_LEXTREEVALIDATION_H

#include "lexTree.h"

typedef enum{
    immediateAddressing = 0,
    directAddressing = 1,
    fixedIndexAddressing = 2,
    directRegisterAddressing = 3,
    emptyOperand,
    errorInImmediateAddressMethod
}addressMethod;

void validateLexTree(lexTree *, labelOrDefinitionList*);

/*------------------------------definition validation functions------------------------------*/

void validateDefinitionLexTree(lexTree *);



/*------------------------------order validation functions------------------------------*/

void validateOrderLexTree(lexTree *, labelOrDefinitionList*);

/*gets operand as string and return addressMethod*/
addressMethod determineAddressMethod(const char *, labelOrDefinitionList*);

/* Return true if there is a label followed by [, then a number or already defined
 * variable, and at last: ], and nothing else follows.*/
boolean isFixedIndexAddressMethod(const char*, labelOrDefinitionList*);

/* Helper function for case of fixed-index-address-method*/
boolean setNCheckAssemblyArrayNIndex(const char* , const char*, labelOrDefinitionList*,
                                     int, int );
/*------------------------------direction validation functions------------------------------*/

void validateDirectionLexTree(lexTree *);

#endif
