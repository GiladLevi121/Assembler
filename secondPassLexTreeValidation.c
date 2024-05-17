
#include <string.h>
#include <stdlib.h>

#include "secondPassLexTreeValidation.h"
#include "lexTree.h"
#include "lexTreeValidation.h"
#include "globaldefinitionsNStructures.h"




void secondPassOrderValidation(lexTree* thisLexTree,
                               labelOrDefinitionList* openingLabelNDefinitionList,
                               entryExternList* entryExternLabelList){
    addressMethod sourceAddressing = determineAddressMethod(
            thisLexTree->content.orderContent.sourceOperand, openingLabelNDefinitionList);
    addressMethod destinationAddressing = determineAddressMethod(
            thisLexTree->content.orderContent.destinationOperand, openingLabelNDefinitionList);

    if (sourceAddressing == directAddressing){
        setValidationStatusForDirectAddressMethodSecondPass(thisLexTree, openingLabelNDefinitionList,
                                                            entryExternLabelList,
                                                            thisLexTree->content.orderContent.sourceOperand);
    }else if(sourceAddressing == fixedIndexAddressing)
        setValidationStatusForFixedIndexAddressMethod(thisLexTree,openingLabelNDefinitionList,
                                                      entryExternLabelList,
                                                      thisLexTree->content.orderContent.sourceOperand);

    if (destinationAddressing == directAddressing){
        setValidationStatusForDirectAddressMethodSecondPass(thisLexTree, openingLabelNDefinitionList,
                                                            entryExternLabelList,
                                                            thisLexTree->content.orderContent.destinationOperand);
    }else if(destinationAddressing == fixedIndexAddressing)
        setValidationStatusForFixedIndexAddressMethod(thisLexTree,openingLabelNDefinitionList,
                                                      entryExternLabelList,
                                                      thisLexTree->content.orderContent.destinationOperand);

}

void setValidationStatusForDirectAddressMethodSecondPass(lexTree* thisLexTree,
                                                         labelOrDefinitionList* openingLabelNDefinitionList,
                                                         entryExternList* entryExternLabelList,
                                                         const char* operand){
    if(isTileAppearInLabelList(operand, openingLabelNDefinitionList)){
        labelNode * foundedNode = getNodeIfAppearInLabelList(operand, openingLabelNDefinitionList);
        if(foundedNode->labelError != valid)
            thisLexTree->error = foundedNode->labelError;
        return;
    }
    if(isTileAppearInEntryExternAsExternDeclarationList(operand, entryExternLabelList)){
        entryExternNode * foundedNode = nodeWithThisTitle(
                operand, entryExternLabelList);
        if(foundedNode->error != valid)
            thisLexTree->error = foundedNode->error;
        return;
    }
    thisLexTree->error = unKnownLabelUsage;
}


void setValidationStatusForFixedIndexAddressMethod(lexTree* thisLexTree,
                                                   labelOrDefinitionList* openingLabelNDefinitionList,
                                                   entryExternList* entryExternLabelList,
                                                   const char* operand){
    const char* indexOfSquareBrackets = strchr(operand, '[');
    char* arrayName;
    if(indexOfSquareBrackets != NULL){
        int arrayNameLength = (int)(indexOfSquareBrackets - operand);
        arrayName = (char*)malloc(sizeof (char) * (arrayNameLength + PADDING_CELL_LEN));
        strncpy(arrayName, operand, arrayNameLength + PADDING_CELL_LEN);
        arrayName[arrayNameLength] = END_OF_STRING;
        setValidationStatusForDirectAddressMethodSecondPass(thisLexTree,openingLabelNDefinitionList,
                                                            entryExternLabelList, arrayName);
    }
}

void setEntryErrorIfNeeded(lexTree *thisLexTree, labelOrDefinitionList * openingLabelNDefinitionList){
    if (thisLexTree->content.directionSentence.type == entryDirection){
        if(!isTileAppearInLabelList(
                thisLexTree->content.directionSentence.content.entryLabel, openingLabelNDefinitionList)){
            thisLexTree->error = declaredEntryLabelButDidntDeclaredLabelInFile;
            return;
        }
    }
}











