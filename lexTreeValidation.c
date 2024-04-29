#include <stdlib.h>

#include "lexTreeValidation.h"
#include "parser.h"
#include "labelNDefineValidation.h"
#include "label.h"


void validateLexTree(lexTree * thisLexTree, labelOrDefinitionList *openingLabelNDefinitionList){
    if (thisLexTree->error != valid)
        return;
    switch (thisLexTree->type) {
        case definition:
            validateDefinitionLexTree(thisLexTree);
            break;
        case order:
            validateOrderLexTree(thisLexTree, openingLabelNDefinitionList);
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


void validateOrderLexTree(lexTree *thisLexTree, labelOrDefinitionList *openingLabelNDefinitionList){
    OrderSentence thisOrderSentence = thisLexTree->content.orderContent;
    addressMethod sourceOperandAddressMethod = determineAddressMethod(
            thisOrderSentence.sourceOperand, openingLabelNDefinitionList);
    addressMethod destinationOperandAddressMethod = determineAddressMethod(
            thisOrderSentence.destinationOperand, openingLabelNDefinitionList);

}


addressMethod determineAddressMethod(const char * operand, labelOrDefinitionList* openingLabelNDefinitionList){
    if(isEmptyString(operand))
        return emptyOperand;
    if(operand[FIRST_INDEX] == '#') {
        const char * restOfTheOperand = &operand[SECOND_CELL_INDEX];
        if(is12BitsLegalNumberAsIs(restOfTheOperand) ||
                getDefinitionValueFromList(openingLabelNDefinitionList, restOfTheOperand) != NULL)
            return immediateAddressing;
        else
            return errorInImmediateAddressMethod;
    }
    if(!isNotLegalTitleWithoutBeginningNEndingWhiteSpaces(operand))
        return directAddressing;
    if (isFixedIndexAddressMethod(operand, openingLabelNDefinitionList))
        return fixedIndexAddressing;
    if(isARegisterName(operand))
        return directRegisterAddressing;
}



boolean isFixedIndexAddressMethod(const char* operand,
                                  labelOrDefinitionList* openingLabelNDefinitionList){
    const char* indexOfSquareBrackets = strchr(operand, '['); /*gets the beginning of the inner index*/
    if(operand[strlen(operand) - LAST_CELL] != ']')
        return false;
    if(indexOfSquareBrackets != NULL){
        int arrayNameLength = (int)(indexOfSquareBrackets - operand);
        int lengthOfInnerIndex = (int)(strlen(operand) - arrayNameLength) - ANOTHER_CELL - LAST_CELL;/*- len('[') - len(']')*/
        if(arrayNameLength <= ZERO_CHARACTERS || lengthOfInnerIndex <= ZERO_CHARACTERS)
            return false; /* Size isn't right*/
        return setNCheckAssemblyArrayNIndex(operand, indexOfSquareBrackets, openingLabelNDefinitionList,
        arrayNameLength, lengthOfInnerIndex);
    }
    return false;
}

boolean setNCheckAssemblyArrayNIndex(const char* operand, const char* indexOfSquareBrackets,
                                     labelOrDefinitionList* openingLabelNDefinitionList,
                                     int arrayNameLength, int lengthOfInnerIndex){
    char* arrayName = (char*)malloc(sizeof (char) * (arrayNameLength + LAST_CELL));
    char* indexName = (char*)malloc(sizeof (char) * (lengthOfInnerIndex));
    strncpy(arrayName, operand, arrayNameLength + LAST_CELL);
    arrayName[arrayNameLength] = END_OF_STRING;
    strncpy(indexName, indexOfSquareBrackets + LAST_CELL , lengthOfInnerIndex);
    indexName[lengthOfInnerIndex] = END_OF_STRING;
    if(isLegalXBitsNumber(indexName, BEGINNING_ASSEMBLY_ARRAY_INDEX, MAX_NUMBERS_IN_DATA_DECLARATION, true) ||
            getDefinitionValueFromListIgnoreWhiteSpaces(openingLabelNDefinitionList, indexName) != NULL){
        free(arrayName);
        free(indexName);
        return true; /*Label naming ok, assembly index is a number < 35 ,>0 or already defined (.define)*/
    }
    free(arrayName);/*Illegal label naming*/
    free(indexName);
    return false;
}

/*------------------------------direction validation functions------------------------------*/

void validateDirectionLexTree(lexTree *thisLexTree){

}















