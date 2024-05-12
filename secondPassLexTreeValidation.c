
#include "secondPassLexTreeValidation.h"
#include "lexTree.h"
#include "lexTreeValidation.h"
#include "globaldefinitionsNStructures.h"




void secondPassMainFunctionValidation(lexTree* thisLexTree,
                                      labelOrDefinitionList* openingLabelNDefinitionList,
                                      entryExternList* entryExternLabelList){
    addressMethod sourceAddressing = determineAddressMethod(
            thisLexTree->content.orderContent.sourceOperand, openingLabelNDefinitionList);
    addressMethod destinationAddressing = determineAddressMethod(
            thisLexTree->content.orderContent.destinationOperand, openingLabelNDefinitionList);

    if (sourceAddressing == directAddressing || sourceAddressing == fixedIndexAddressing)
        setValidationStatusForDirectAddressMethod(thisLexTree, openingLabelNDefinitionList,
                                                  entryExternLabelList,
                                                  thisLexTree->content.orderContent.sourceOperand);
    if (destinationAddressing == directAddressing)
        setValidationStatusForDirectAddressMethod(thisLexTree, openingLabelNDefinitionList,
                                                  entryExternLabelList,
                                                  thisLexTree->content.orderContent.destinationOperand);
}

void setValidationStatusForDirectAddressMethod(lexTree* thisLexTree,
                                               labelOrDefinitionList* openingLabelNDefinitionList,
                                               entryExternList* entryExternLabelList,
                                               const char* operand){
    printf("here");
    if(isTileAppearInLabelList(
            operand, openingLabelNDefinitionList)){
        labelNode * foundedNode =
                getNodeIfAppearInLabelList(
                        operand, openingLabelNDefinitionList);
        if(foundedNode->labelError != valid)
            thisLexTree->error = conflictNaming;
        return;
    }
    printf("haha");
    if(isTileAppearInEntryExternAsExternDeclarationList(
            operand, entryExternLabelList)){
        entryExternNode * foundedNode = nodeWithThisTitle(
                operand, entryExternLabelList);
        if(foundedNode->error != valid)
            thisLexTree->error = conflictNaming;
        return;
    }
    thisLexTree->error = unKnownLabelUsage;
}