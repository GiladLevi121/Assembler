

#ifndef ASSEMBLER_LEXTREEVALIDATION_H
#define ASSEMBLER_LEXTREEVALIDATION_H

#include "SettingLexTree.h"

typedef enum{
    immediateAddressing = 0,
    directAddressing = 1,
    fixedIndexAddressing = 2,
    directRegisterAddressing = 3,
    emptyOperand,
    errorInImmediateAddressMethod,
    criticalError
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

/* Check if the opcode and the addressing method are compatible according to
 * page 33 in the mmn. Sets error if needed*/
void setErrorForInCompatibleAddressingMethods(lexTree*, addressMethod, addressMethod);

/* Checker for "setErrorForInCompatibleAddressingMethods" case*/
void setErrorForImmediateErrorIfNeeded(lexTree*, addressMethod, addressMethod);

/* Checker for "setErrorForInCompatibleAddressingMethods" case*/
void setErrorForMovAddSubIfNeeded(lexTree*, addressMethod, addressMethod);

/* Checker for "setErrorForInCompatibleAddressingMethods" case*/
void setErrorForNotClrIncDecRedIfNeeded(lexTree*, addressMethod, addressMethod);

/* Checker for "setErrorForInCompatibleAddressingMethods" case*/
void setErrorForLeaIfNeeded(lexTree*, addressMethod, addressMethod);

/* Checker for "setErrorForInCompatibleAddressingMethods" case*/
void setErrorForJmpBneJsrIfNeeded(lexTree*, addressMethod, addressMethod);

/* Checker for "setErrorForInCompatibleAddressingMethods" case*/
void setErrorForPrnJsrIfNeeded(lexTree*, addressMethod, addressMethod);

/*------------------------------direction validation functions------------------------------*/

/* Main function for direction validation*/
void validateDirectionLexTree(lexTree *);

/* Main function for data direction validation*/
void validateDataDirectionSentence(lexTree *);

/* Main function for string direction validation*/
void validateStringDirection(lexTree *);

/* Main function for extern & entry direction validation*/
void validateExternOrEntry(lexTree *thisLexTree);


#endif
