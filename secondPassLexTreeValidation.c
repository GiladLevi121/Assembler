
#include "secondPassLexTreeValidation.h"
#include "lexTree.h"
#include "lexTreeValidation.h"
#include "globaldefinitionsNStructures.h"




void secondPassMainFunctionValidation(lexTree* thisLexTree,
                                      labelOrDefinitionList* openingLabelNDefinitionList,
                                      entryExternList* entryExternLabelList){
    if (determineAddressMethod(
            thisLexTree->content.orderContent.sourceOperand, openingLabelNDefinitionList) == directAddressing)
        setValidationStatusForDirectAddressMethod(thisLexTree, openingLabelNDefinitionList,
                                                  entryExternLabelList,
                                                  thisLexTree->content.orderContent.sourceOperand);
    if (determineAddressMethod(
            thisLexTree->content.orderContent.destinationOperand, openingLabelNDefinitionList) == directAddressing)
        setValidationStatusForDirectAddressMethod(thisLexTree, openingLabelNDefinitionList,
                                                  entryExternLabelList,
                                                  thisLexTree->content.orderContent.destinationOperand);
}

void setValidationStatusForDirectAddressMethod(lexTree* thisLexTree,
                                               labelOrDefinitionList* openingLabelNDefinitionList,
                                               entryExternList* entryExternLabelList,
                                               const char* operand){
    if(isTileAppearInLabelList(
            operand, openingLabelNDefinitionList)){
        labelNode * foundedNode =
                getNodeIfAppearInLabelList(
                        operand, openingLabelNDefinitionList);
        if(foundedNode->labelError != valid)
            thisLexTree->error = conflictNaming;
        return;
    }
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