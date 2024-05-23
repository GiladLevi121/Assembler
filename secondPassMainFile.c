#include <string.h>

#include "secondPassMainFile.h"
#include "memoryImage.h"
#include "globaldefinitionsNStructures.h"
#include "label.h"
#include "filesUtil.h"
#include "lexTree.h"
#include "SettingLexTree.h"
#include "lexTreeValidation.h"
#include "secondPassLexTreeValidation.h"
#include "secondPassCodding.h"
#include "baseTransitionUtiles.h"

void runSecondPass(char *fileName, labelOrDefinitionList* openingLabelNDefinitionList,
                   entryExternList * entryExternLabelList, memoryImage* fileMemoryImage,
                   boolean* fileErrorDetected){
    FILE *filePointer = openFileByName(fileName, AM_ENDING, "r");
    assemblyLineCode *newAssemblyLine;
    int instructionCounter = ZEROISE_COUNTER;
    while ((newAssemblyLine = getNextAssemblyLine(filePointer)) != NULL){
        secondPassEveryLineOfAssemblyOperations(newAssemblyLine, ++instructionCounter,
                                               openingLabelNDefinitionList, entryExternLabelList,
                                               fileMemoryImage, fileErrorDetected, fileName);
    }
    fclose(filePointer);
    if (*fileErrorDetected == false)
        createOutputFiles(fileMemoryImage, entryExternLabelList, fileName);
}


void secondPassEveryLineOfAssemblyOperations(assemblyLineCode *newAssemblyLine, int instructionCounter,
                                             labelOrDefinitionList* openingLabelNDefinitionList,
                                             entryExternList * entryExternLabelList,
                                             memoryImage* fileMemoryImage, boolean* fileErrorDetected,
                                             const char* fileName){
    lexTree *thisLexTree = lexTreeConstructor(newAssemblyLine, instructionCounter,
                                              fileMemoryImage->currentlyWordsInDataImage,
        /* PC + amount of words in CI <=>*/   (fileMemoryImage->currentlyWordsInCodeImage + fileMemoryImage->PC));
    validateLexTree(thisLexTree, openingLabelNDefinitionList);
    if(thisLexTree->error == valid){
        additionValidationSecondPass(thisLexTree, openingLabelNDefinitionList, entryExternLabelList);
        if(thisLexTree->error == valid && *fileErrorDetected == false)
            secondPassCoding(thisLexTree, openingLabelNDefinitionList, entryExternLabelList, fileMemoryImage);
        else{
            if (thisLexTree->error != valid){
                char* outputFile = connect2strings(fileName, AM_ENDING);
                *fileErrorDetected = true;
                printErrorToUser(outputFile, thisLexTree->error, thisLexTree->InstructionCounter);
                free(outputFile);
            }
        }
    }
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
    FILE * entFilePointer, *extFilePointer;
    entryExternNode * current;
    boolean entFileCreationFlag, extFileCreationFlag;
    current = entryExternLabelList->head;
    entFileCreationFlag = false;
    extFileCreationFlag = false;
    while (current != NULL){
        if (entFileCreationFlag == false && current->type == entryDeclaration && current->error == valid){/*enters if needed to open file to write*/
            entFilePointer = openFileByName(fileName, ENT_ENDING, "w");
            entFileCreationFlag = true;
        }
        if (extFileCreationFlag == false && current->type == externDeclaration && current->error == valid){/*enters if needed to open file to write*/
            extFilePointer = openFileByName(fileName, EXT_ENDING, "w");
            extFileCreationFlag = true;
        }
        if( entFileCreationFlag == true && current->type == entryDeclaration && current->error == valid)
            printLineToEntFile(current, entFilePointer);
        if (extFileCreationFlag == true && current->type == externDeclaration && current->error == valid)
            printLineToeExtFile(current, extFilePointer);
        current = (entryExternNode *) current->next;
    }
    closeFilesIfNeed(entFilePointer, extFilePointer, entFileCreationFlag, extFileCreationFlag);
}

void closeFilesIfNeed(FILE * entFilePointer, FILE *extFilePointer,
                      boolean entFileCreationFlag, boolean extFileCreationFlag){
    if(entFileCreationFlag == true)
        fclose(entFilePointer);
    if (extFileCreationFlag == true)
        fclose(extFilePointer);
}

void printLineToEntFile(entryExternNode* thisNode, FILE *entFilePointer){
    char* entLine = (char*) malloc(FIRST_LINE_MAX_LENGTH * sizeof(char));
    if(thisNode->lastOfItsTypeInList)
        sprintf(entLine,"%s\t%s", thisNode->title, getDeclaredEntryLine(thisNode));
    else
        sprintf(entLine,"%s\t%s\n", thisNode->title, getDeclaredEntryLine(thisNode));
    writeStringToFile(entFilePointer, entLine);
    free(entLine);
}

void printLineToeExtFile(entryExternNode* thisNode, FILE *extFilePointer){
    int i = ZEROISE_COUNTER;
    for(; i < thisNode->amountOfLines; i++){
        char* extLine = (char*) malloc(FIRST_LINE_MAX_LENGTH * sizeof(char));
        if(thisNode->lastOfItsTypeInList && i + ANOTHER_LINE == thisNode->amountOfLines)
            sprintf(extLine,"%s\t%s", thisNode->title, thisNode->content.usedLines[i]);
        else
            sprintf(extLine,"%s\t%s\n", thisNode->title, thisNode->content.usedLines[i]);
        writeStringToFile(extFilePointer, extLine);
        free(extLine);
    }
}

void createObjFile(memoryImage *fileMemoryImage, const char * fileName){
    FILE * objFilePointer = openFileByName(fileName, OB_ENDING, "w");
    writeFirstLineToObjFile(objFilePointer, fileMemoryImage);
    printCodeImageToFile(objFilePointer, fileMemoryImage);
    printDataImageToFile(objFilePointer, fileMemoryImage);
    fclose(objFilePointer);
}

void printDataImageToFile(FILE* objFilePointer, memoryImage* fileMemoryImage){
    int i = ZEROISE_COUNTER;
    for(; i < fileMemoryImage->currentlyWordsInDataImage; i++){
        if(fileMemoryImage->dataImage[i] != NULL){
            char* encryptedLine = memoryWordToEncrypted4Base(fileMemoryImage->dataImage[i]);
            char* completeLine = getFormattedOBJLine(
                    encryptedLine,
                    (int)(i + FIRST_FREE_WORD + fileMemoryImage->currentlyWordsInCodeImage));
            if(i + LAST_LINE_TO_ERASE == fileMemoryImage->currentlyWordsInDataImage) /*removing last empty line*/
                completeLine[strlen(completeLine) - PADDING_CELL_LEN] = END_OF_STRING;
            writeStringToFile(objFilePointer, completeLine);
            free(encryptedLine);
            free(completeLine);
        }
    }
}

void printCodeImageToFile(FILE* objFilePointer, memoryImage* fileMemoryImage){
    int i = ZEROISE_COUNTER;
    for(; i < fileMemoryImage->currentlyWordsInCodeImage; i++){
        if(fileMemoryImage->codeImage[i] != NULL){
            char* encryptedLine = memoryWordToEncrypted4Base(fileMemoryImage->codeImage[i]);
            char* completeLine = getFormattedOBJLine(encryptedLine, i + FIRST_FREE_WORD);
            if((i + LAST_LINE_TO_ERASE == fileMemoryImage->currentlyWordsInCodeImage)
               && fileMemoryImage->currentlyWordsInDataImage == ZERO_TOKENS) /*removing last empty line*/
                completeLine[strlen(completeLine) - PADDING_CELL_LEN] = END_OF_STRING;
            writeStringToFile(objFilePointer, completeLine);
            free(encryptedLine);
            free(completeLine);
        }
    }
}

char* getFormattedOBJLine(const char* encryptedFourBaseWord, int lineNumber){
    char* outputLine = (char*) malloc(OBJ_LINE_LENGTH_WITH_PADDING * sizeof(char));
    if (lineNumber < NUMBER_TO_REPRESENT_4_DIGITS_NUMBER)
        sprintf(outputLine, "0%d %s", lineNumber, encryptedFourBaseWord);
    else
        sprintf(outputLine, "%d %s", lineNumber, encryptedFourBaseWord);
    return outputLine;
}

void writeFirstLineToObjFile(FILE* pointerToObj, memoryImage* thisMemoryImg){
    char* firstLine = (char*) malloc(FIRST_LINE_MAX_LENGTH * sizeof(char));
    sprintf(firstLine, "  %d %d\n",
            (int)(thisMemoryImg->currentlyWordsInCodeImage), (int)(thisMemoryImg->currentlyWordsInDataImage));
    writeStringToFile(pointerToObj, firstLine);
    free(firstLine);
}



