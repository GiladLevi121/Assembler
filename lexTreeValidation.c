//
// Created by Levi Gila'ad on 21/03/2024.
//

#include "lexTreeValidation.h"
#include "parrser.h"


void validateLexTree(lexTree * thisLexTree){
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
    checkForForbiddenOpeningLabel(thisLexTree);
    if(!is14BitsLegalNumber(thisLexTree->content.definitionContent.value)){
        thisLexTree->error = illegalNumber;
        return;
    }

}

void checkForForbiddenOpeningLabel(lexTree* thisLexTree){
    if (thisLexTree->potentialLabel != NULL)
        thisLexTree->error = definitionCantHaveALabel;
}



/*------------------------------order validation functions------------------------------*/


void validateOrderLexTree(lexTree *thisLexTree){

}


/*------------------------------direction validation functions------------------------------*/

void validateDirectionLexTree(lexTree *thisLexTree){

}















