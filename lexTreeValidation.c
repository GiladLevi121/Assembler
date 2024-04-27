

#include "lexTreeValidation.h"
#include "parser.h"
#include "labelNDefineValidation.h"


void validateLexTree(lexTree * thisLexTree/*, definition list*/){
    if (thisLexTree->error != valid)
        return;
    switch (thisLexTree->type) {
        case definition:
            validateDefinitionLexTree(thisLexTree);
            break;
        case order:
            validateOrderLexTree(thisLexTree);
            break;
        case direction:
            validateDirectionLexTree(thisLexTree);
            break;
        default:
            if (thisLexTree->error != undefinedCommand)
                thisLexTree->error = undefinedCommand;
    }
}

/*------------------------------definition validation functions------------------------------*/


void validateDefinitionLexTree(lexTree *thisLexTree){
    if (thisLexTree->potentialLabel != NULL)
        thisLexTree->error = definitionCantHaveALabel;
    if(!is14BitsLegalNumberIgnoreWhiteSpaces(thisLexTree->content.definitionContent.value))
        thisLexTree->error = illegalNumber;
    if (isNotLegalTitle(thisLexTree->content.definitionContent.name))
        thisLexTree->error = definitionNamingIsIllegal;
    if(thisLexTree->content.definitionContent.value[FIRST_INDEX] == END_OF_STRING)
        thisLexTree->error = missingArgument;
}


/*------------------------------order validation functions------------------------------*/


void validateOrderLexTree(lexTree *thisLexTree){
    OrderSentence thisOrderSentence = thisLexTree->content.orderContent;
    addressMethod firstOperandAddressMethod = determineAddressMethod(thisOrderSentence.firstOperand);
    addressMethod secondOperandAddressMethod = determineAddressMethod(thisOrderSentence.secondOperand);
}


addressMethod determineAddressMethod(const char * operand){
    if(operand[FIRST_INDEX] == '#') {
        if(isKnownLabelOrLegalNumber(&operand[SECOND_CELL_INDEX]/*, */))
            return immediate;
        else
            return errorInImmediateAddressMethod;
    }
    //  if(legal label (steted already or will be stat) => return direct
    //  if (defined sentence
    if(isARegisterName(operand))
        return directRegister;
}

boolean isKnownLabelOrLegalNumber(const char * restOfTheOperand/*, labelOrDefinitionList* definitionList*/){
    if(is12BitsLegalNumberAsIs(restOfTheOperand))
        return true;
    return false;
}

/*------------------------------direction validation functions------------------------------*/

void validateDirectionLexTree(lexTree *thisLexTree){

}















