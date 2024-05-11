
#include "secondPassMainFile.h"
#include "memoryImage.h"
#include "globaldefinitionsNStructures.h"
#include "label.h"
#include "filesUtil.h"
#include "lexTree.h"
#include "SettingLexTree.h"
#include "lexTreeValidation.h"
#include "firstPassMainFile.h"
#include "secondPassLexTreeValidation.h"

void runSecondPass(char *fileName, /*boolean errorExist,*/
                   labelOrDefinitionList* openingLabelNDefinitionList,
                   entryExternList * entryExternLabelList, memoryImage* fileMemoryImage){
    FILE *filePointer = openFileByName(fileName, AS_ENDING, "r");
    assemblyLineCode *newAssemblyLine;
    int instructionCounter = ZEROISE_COUNTER;
    if(filePointer == NULL){
        printf("Failed to open file %s, please make sure that file with exact"
               " name is in the project file\n", fileName);
        return;
    }
    while ((newAssemblyLine = getNextAssemblyLine(filePointer)) != NULL){
        secondPassEveryLineOfAssemblyOperations(newAssemblyLine,
                                               ++instructionCounter,
                                               openingLabelNDefinitionList,
                                               entryExternLabelList,
                                               fileMemoryImage);
    }

    fclose(filePointer);
}


void secondPassEveryLineOfAssemblyOperations(assemblyLineCode *newAssemblyLine, int instructionCounter,
                                             labelOrDefinitionList* openingLabelNDefinitionList,
                                             entryExternList * entryExternLabelList,
                                             memoryImage* fileMemoryImage){
    lexTree *thisLexTree = lexTreeConstructor(newAssemblyLine, instructionCounter,
                                              fileMemoryImage->currentlyWordsInDataImage,
        /* PC + amount of words in CI <=>*/   (fileMemoryImage->currentlyWordsInCodeImage + fileMemoryImage->PC));
    validateLexTree(thisLexTree, openingLabelNDefinitionList);
    additionValidationSecondPass(thisLexTree, openingLabelNDefinitionList, entryExternLabelList);

}


void additionValidationSecondPass(lexTree *thisLexTree,labelOrDefinitionList *openingLabelNDefinitionList,
                                  entryExternList* entryExternLabelList){
    if (thisLexTree->error != valid)
        return;
    else{
        if (thisLexTree->type == order)
            secondPassMainFunctionValidation(thisLexTree, openingLabelNDefinitionList, entryExternLabelList);
    }
}

