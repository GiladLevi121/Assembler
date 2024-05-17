
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
#include "secondPassCodding.h"
#include "baseTransitionUtiles.h"

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
    createOutputFiles(fileMemoryImage, entryExternLabelList);
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
    secondPassCoding(thisLexTree, openingLabelNDefinitionList, entryExternLabelList, fileMemoryImage);
}


void additionValidationSecondPass(lexTree *thisLexTree,labelOrDefinitionList *openingLabelNDefinitionList,
                                  entryExternList* entryExternLabelList){
    if (thisLexTree->error != valid)
        return;
    else{
        if(thisLexTree->type == direction)
            setEntryErrorIfNeeded(thisLexTree, openingLabelNDefinitionList);
        if (thisLexTree->type == order)
            secondPassOrderValidation(thisLexTree, openingLabelNDefinitionList, entryExternLabelList);
    }
}

void secondPassCoding(lexTree *thisLexTree, labelOrDefinitionList *openingLabelNDefinitionList,
                      entryExternList *entryExternLabelList, memoryImage* fileMemoryImage){
    if(thisLexTree->error != valid)
        return;
    if (thisLexTree->type == order)
        secondPassOrderCodingInToMemoryIfNeeded(thisLexTree, openingLabelNDefinitionList,
                                                entryExternLabelList, fileMemoryImage);
}



void createOutputFiles(memoryImage *fileMemoryImage, entryExternList *entryExternLabelList, const char* fileName){
    createObjFile(fileMemoryImage, fileName);
    createEntNExtFile(entryExternLabelList, fileName);
}

void createEntNExtFile(entryExternList *entryExternLabelList, const char* fileName){

}

void createObjFile(memoryImage *fileMemoryImage, const char * fileName){
    FILE * objFilePointer = openFileByName(fileName, OB_ENDING, "w");
    int i = ZEROISE_COUNTER;
    for(; i < fileMemoryImage->currentlyWordsInCodeImage; i++){
        if(fileMemoryImage->codeImage[i] != NULL)
            printf("%d) %s\n", i, memoryWordToEncrypted4Base(fileMemoryImage->codeImage[i]));
        else
            printf("%d) NULL\n", i);
    }
    i = ZEROISE_COUNTER;
    for(; i < fileMemoryImage->currentlyWordsInDataImage; i++){
        if(fileMemoryImage->dataImage[i] != NULL)
            printf("%d) %s\n", i, memoryWordToEncrypted4Base(fileMemoryImage->dataImage[i]));
        else
            printf("%d) NULL\n", i);
    }
}






