//
// Created by Levi Gila'ad on 21/03/2024.
//

#include "lexTreeValidation.h"


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

void validateDefinitionLexTree(lexTree *thisLexTree){
    /*checkForForbiddenOpeningLabel();*/

}

void validateOrderLexTree(lexTree *thisLexTree){

}

void validateDirectionLexTree(lexTree *thisLexTree){

}















