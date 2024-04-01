

#include "lexTreeValidation.h"
#include "parser.h"
#include "labelNDefineValidation.h"


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
    if (thisLexTree->potentialLabel != NULL)
        thisLexTree->error = definitionCantHaveALabel;
    if(!is14BitsLegalNumber(thisLexTree->content.definitionContent.value))
        thisLexTree->error = illegalNumber;
    if (isNotLegalTitle(thisLexTree->content.definitionContent.name))
        thisLexTree->error = definitionNamingIsIllegal;
    if(thisLexTree->content.definitionContent.value[FIRST_INDEX] == END_OF_STRING)
        thisLexTree->error = missingArgument;
}


/*------------------------------order validation functions------------------------------*/


void validateOrderLexTree(lexTree *thisLexTree){

}


/*------------------------------direction validation functions------------------------------*/

void validateDirectionLexTree(lexTree *thisLexTree){

}















