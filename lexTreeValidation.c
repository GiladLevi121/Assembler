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
            validateDirectionLexTree(thisLexTree, openingLabelNDefinitionList);
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
    setErrorForInCompatibleAddressingMethods(thisLexTree, sourceOperandAddressMethod,
                    destinationOperandAddressMethod);
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
    return criticalError;
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

void setErrorForInCompatibleAddressingMethods(lexTree* thisLexTree,
                                              addressMethod sourceOperandAddressMethod,
                                              addressMethod destinationOperandAddressMethod) {
    commandOpcode thisOpcode = thisLexTree->content.orderContent.opcode;
    setErrorForImmediateErrorIfNeeded(thisLexTree, sourceOperandAddressMethod,
                                      destinationOperandAddressMethod);
    /* AddressMethods are legal, check combination*/
    if (thisOpcode == mov || thisOpcode == add || thisOpcode == sub)
        setErrorForMovAddSubIfNeeded(thisLexTree, sourceOperandAddressMethod,
                destinationOperandAddressMethod);

    else if (thisOpcode == cmp){
        if (sourceOperandAddressMethod == emptyOperand ||
            destinationOperandAddressMethod == emptyOperand)
            thisLexTree->error = missingOperand;
    }
    else if (thisOpcode == not || thisOpcode == clr ||
    thisOpcode == inc || thisOpcode == dec || thisOpcode == red)
        setErrorForNotClrIncDecRedIfNeeded(thisLexTree, sourceOperandAddressMethod,
                                           destinationOperandAddressMethod);
    else if(thisOpcode == lea)
        setErrorForLeaIfNeeded(thisLexTree, sourceOperandAddressMethod,
                               destinationOperandAddressMethod);

    else if(thisOpcode == jmp || thisOpcode == bne || thisOpcode == jsr)
        setErrorForJmpBneJsrIfNeeded(thisLexTree, sourceOperandAddressMethod,
                               destinationOperandAddressMethod);
    else if(thisOpcode == prn)
        setErrorForPrnJsrIfNeeded(thisLexTree, sourceOperandAddressMethod,
                                  destinationOperandAddressMethod);
    else if (thisOpcode == rts || thisOpcode == hlt) {
        if (sourceOperandAddressMethod != emptyOperand ||
            destinationOperandAddressMethod != emptyOperand)
            thisLexTree->error = toMannyOperands;
    }else
        return;
}

/* Checker for "setErrorForInCompatibleAddressingMethods" case*/
void setErrorForImmediateErrorIfNeeded(lexTree* thisLexTree,
                                       addressMethod sourceOperandAddressMethod,
                                       addressMethod destinationOperandAddressMethod){
    if (sourceOperandAddressMethod == errorInImmediateAddressMethod ||
        sourceOperandAddressMethod == criticalError)
        thisLexTree->error = sourceOperandAddressMethodError;
    if (destinationOperandAddressMethod == errorInImmediateAddressMethod ||
        destinationOperandAddressMethod == criticalError)
        thisLexTree->error = destinationOperandAddressMethodError;
}

void setErrorForMovAddSubIfNeeded(lexTree* thisLexTree,
                                  addressMethod sourceOperandAddressMethod,
                                  addressMethod destinationOperandAddressMethod){
    if (sourceOperandAddressMethod == emptyOperand)  /* !allowed methods*/
        thisLexTree->error = missingOperand;

    if (destinationOperandAddressMethod == emptyOperand ||
    destinationOperandAddressMethod == immediateAddressing)
        thisLexTree->error = destinationOperandAddressMethodError;
}

void setErrorForNotClrIncDecRedIfNeeded(lexTree* thisLexTree,
                                        addressMethod sourceOperandAddressMethod,
                                        addressMethod destinationOperandAddressMethod){
    if (sourceOperandAddressMethod != emptyOperand)
        thisLexTree->error = inCompatibleOperand;
    if(destinationOperandAddressMethod == immediateAddressing ||
       destinationOperandAddressMethod == emptyOperand)
        thisLexTree->error = destinationOperandAddressMethodError;
}

void setErrorForLeaIfNeeded(lexTree* thisLexTree,
                            addressMethod sourceOperandAddressMethod,
                            addressMethod destinationOperandAddressMethod){
    if(sourceOperandAddressMethod != directAddressing &&
       sourceOperandAddressMethod != fixedIndexAddressing)
        thisLexTree->error = sourceOperandAddressMethodError;
    if(destinationOperandAddressMethod == emptyOperand ||
       destinationOperandAddressMethod == immediateAddressing)
        thisLexTree->error = destinationOperandAddressMethodError;
}

void setErrorForJmpBneJsrIfNeeded(lexTree* thisLexTree,
                                  addressMethod sourceOperandAddressMethod,
                                  addressMethod destinationOperandAddressMethod){
    if(sourceOperandAddressMethod != emptyOperand)
        thisLexTree->error = sourceOperandAddressMethodError;
    if(destinationOperandAddressMethod != directAddressing &&
       destinationOperandAddressMethod != directRegisterAddressing)
        thisLexTree->error = destinationOperandAddressMethodError;
}

void setErrorForPrnJsrIfNeeded(lexTree* thisLexTree,
                               addressMethod sourceOperandAddressMethod,
                               addressMethod destinationOperandAddressMethod){
    if(sourceOperandAddressMethod != emptyOperand)
        thisLexTree->error = inCompatibleOperand;
    if(destinationOperandAddressMethod == emptyOperand)
        thisLexTree->error = missingOperand;
}

/*------------------------------direction validation functions------------------------------*/

void validateDirectionLexTree(lexTree *thisLexTree, labelOrDefinitionList* openingNDefinitionList){
    DirectionSentence thisDirectionSentence;
    thisDirectionSentence = thisLexTree->content.directionSentence;
    if(thisDirectionSentence.type == dataDirection)
        validateDataDirectionSentence(thisLexTree, openingNDefinitionList);
    else if (thisDirectionSentence.type == stringDirection)
        validateStringDirection(thisLexTree);
    else
        validateExternOrEntry(thisLexTree);
}

void validateDataDirectionSentence(lexTree *thisLexTree, labelOrDefinitionList* openingNDefinitionList){
    char ** dataArguments = thisLexTree->content.directionSentence.content.dataDirection;
    int i = ZEROISE_COUNTER;
    /*how many pointers*/
    size_t pointersInDoublePointer = sizeof (dataArguments)/ sizeof (dataArguments[i]);
    while(i < pointersInDoublePointer){
        if (!is14BitsLegalNumberIgnoreWhiteSpaces(dataArguments[i]) ||
                getDefinitionValueFromListIgnoreWhiteSpaces(
                        openingNDefinitionList, dataArguments[i]) == NULL){
        /* not legal number and not defined already*/
        thisLexTree->error = incompatibleArgumentForDataDeclaration;
        }
        i++;
    }
}

void validateStringDirection(lexTree *thisLexTree){
    const char * assemblyStr = thisLexTree->content.directionSentence.content.stringContent;
    size_t lastIndex = strlen(assemblyStr) - LAST_CELL;
    if(assemblyStr[FIRST_INDEX] != '"' || assemblyStr[lastIndex] != '"' || lastIndex == FIRST_INDEX)
        thisLexTree->error = missingQuotationMarksInStringDeclaration;
}

void validateExternOrEntry(lexTree *thisLexTree){
    const char* title;
    if (thisLexTree->content.directionSentence.type == entryDirection)
        title = thisLexTree->content.directionSentence.content.entryLabel;
    else
        title = thisLexTree->content.directionSentence.content.externLabel;
    if(isNotLegalTitle(title))
        thisLexTree->error = invalidNameForEntryOrExternDeclaration;
}










