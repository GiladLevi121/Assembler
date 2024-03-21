

#include <string.h>
#include <stdlib.h>

#include "lexTree.h"
#include "label.h"
#include "parrser.h"

/*order match DirectionSentence struct in lexTree.h*/
const char* directionsTypeIdentifier[] = {".string",".data",
                                          ".entry", ".extern"};


const char* opcodeString[] =          {"mov", "cmp","add","sub",
                                       "lea","not","clr","inc",
                                       "dec","jmp","bne","red",
                                       "prn","jsr","rts","hlt"};

lexTree *lexTreeConstructor(const assemblyLineCode *inputAssemblyLine, int pc){
    lexTree *newLexTree = (lexTree*)malloc(sizeof (lexTree));
    newLexTree->PC = pc;
    if(inputAssemblyLine->status == lineOutOfBounds){
        newLexTree->error = lineLengthIsTooLong;
        return newLexTree;
    }
    newLexTree->potentialLabel = labelNodeConstructor(inputAssemblyLine);
    newLexTree->rawLine = inputAssemblyLine;
    newLexTree->rawLineInnerIndex = ZEROISE_COUNTER;
    setLexTreeType(newLexTree);
    setLexTreeContent(newLexTree);
    setLabelContent(newLexTree/*, CI, DI*/);
    return newLexTree;
}

void setLabelContent(lexTree *newLexTree){

}

void resetInnerIndex(lexTree* thisLexTree, size_t plusIndex){
    thisLexTree->rawLineInnerIndex = thisLexTree->rawLineInnerIndex + plusIndex;
}

void setLexTreeType(lexTree* thisLexTree){
    const char *rawLineContent = thisLexTree->rawLine->content;
    size_t indexAfterLabel = getLabelLengthWithLabelIdentifier(thisLexTree->potentialLabel);
    size_t indexAfterLabelWithoutWhiteSpaces = indexAfterLabel;
    int plusIndex = findFirstNonWhitespaceIndex(&rawLineContent[indexAfterLabel]);
    if(plusIndex != ALL_WHITE_SPACES)
        indexAfterLabelWithoutWhiteSpaces = indexAfterLabel + plusIndex;
    resetInnerIndex(thisLexTree, indexAfterLabelWithoutWhiteSpaces);
    if (rawLineContent[thisLexTree->rawLineInnerIndex] != DIRECTION_OR_DEFINITION_SENTENCE_KNOT)
        thisLexTree->type = order;
    else if(!memcmp(&rawLineContent[thisLexTree->rawLineInnerIndex],
                    DEFINE_SENTENCE_IDENTIFIER,
                    DEFINE_SENTENCE_IDENTIFIER_LENGTH))
        thisLexTree->type = definition;
    else
        thisLexTree->type = direction;
}

void setLexTreeContent(lexTree *newLexTree){
    commasValidation(newLexTree);
    if(newLexTree->type == direction)
        setDirectionLexTreeContent(newLexTree);
    else if(newLexTree->type == order)
        setOrderLexTreeContent(newLexTree);
    else
        setDefinitionLexTreeContent(newLexTree);
}

void commasValidation(lexTree *newLexTree){
    const char* restOfTheAssemblyCodeLine = &newLexTree->rawLine->content[newLexTree->rawLineInnerIndex];
    if (twoConsecutiveCommas(restOfTheAssemblyCodeLine))
        newLexTree->error = twoConsecutiveCommasFound;
    if (endsWithComma(restOfTheAssemblyCodeLine))
        newLexTree->error = commaIsTheLastChar;

}

void setDirectionLexTreeContent(lexTree *newLexTree){
    setDirectionSentenceType(newLexTree);
    directionDeclarationCheck(newLexTree);
    setDirectionContent(newLexTree);
}

void directionDeclarationCheck(lexTree* newLexTree){
    const char *lineAfterDirection = &newLexTree->rawLine->content[newLexTree->rawLineInnerIndex];
    int firstNonWhitespaceIndex = findFirstNonWhitespaceIndex(lineAfterDirection);
    char firstNonWhiteSpaceAfterDirectionChar;
    if(!isThereMandatoryWhiteSpace(lineAfterDirection)){
        newLexTree->error = mandatoryWhiteCharAfterKeyWord;
        return;
    }
    if(firstNonWhitespaceIndex != ALL_WHITE_SPACES){
        firstNonWhiteSpaceAfterDirectionChar =  newLexTree->rawLine->content[firstNonWhitespaceIndex];
        if( firstNonWhiteSpaceAfterDirectionChar == ','){
            newLexTree->error = commaCantBeTheFirsCharAfterDirectionDeclaration;
            return;
        }
    }
}

void setDirectionSentenceType(lexTree *newLexTree){
    const char *rawLineContent = newLexTree->rawLine->content;
    int directionTypeIndex = ZEROISE_COUNTER;
    size_t numberOfDirectionTypes = sizeof(directionsTypeIdentifier) /
            sizeof(directionsTypeIdentifier[FIRST_INDEX]);
    for(;directionTypeIndex < numberOfDirectionTypes; directionTypeIndex++){
        if(!memcmp(&rawLineContent[newLexTree->rawLineInnerIndex],
                   directionsTypeIdentifier[directionTypeIndex],
                   strlen(directionsTypeIdentifier[directionTypeIndex]))){
            newLexTree->content.directionSentence.type = directionTypeIndex;
            resetInnerIndex(newLexTree, strlen(directionsTypeIdentifier[directionTypeIndex]));
            return;
        }
    }
    newLexTree->content.directionSentence.type = unfamiliarDirection;
}

void setDirectionContent(lexTree *newLexTree){
    if(newLexTree->content.directionSentence.type == dataDirection){
        setDataDirectionContent(newLexTree);
        /*trim starting and ending white spaces, validation*/
    }
    else if (newLexTree->content.directionSentence.type == stringDirection)
        setStringDirectionContent(newLexTree);
    else if(newLexTree->content.directionSentence.type == entryDirection ||
             newLexTree->content.directionSentence.type == externDirection){
        setEntryNExternContent(newLexTree);
    }
    else
        newLexTree->error = undefinedDirection;
}

void setEntryNExternContent(lexTree * newLexTree){
    const char *relevantRawLine = &newLexTree->rawLine->content[newLexTree->rawLineInnerIndex];
    int contentStartingIndex = findFirstNonWhitespaceIndex(relevantRawLine);
    int contentEndingIndex = findFirstNonWhitespaceIndexFromEnd(relevantRawLine);
    int charactersInContent = contentEndingIndex - contentStartingIndex;
    if(charactersInContent < ZERO_CHARACTERS){
        newLexTree->error = problemInLabelDefinition;
        return;
    }
    if (newLexTree->content.directionSentence.type == entryDirection){
        memcpy(newLexTree->content.directionSentence.content.entryLabel,
               &relevantRawLine[contentStartingIndex],
               charactersInContent);
        newLexTree->content.directionSentence.content.entryLabel[charactersInContent] = END_OF_STRING;
    }
    else{
        memcpy(newLexTree->content.directionSentence.content.externLabel,
               &relevantRawLine[contentStartingIndex],
               charactersInContent);
        newLexTree->content.directionSentence.content.externLabel[charactersInContent] = END_OF_STRING;
    }
    resetInnerIndex(newLexTree, contentEndingIndex);
}

void setDataDirectionContent(lexTree* newLexTree){
    size_t counter = ZEROISE_COUNTER;
    const char* rawLine = newLexTree->rawLine->content;
    char* token;
    do {
        size_t rawLineIndex = newLexTree->rawLineInnerIndex;
        if(strchr(&rawLine[rawLineIndex], ',') != NULL){
            token = getTokensUpToChar(&rawLine[rawLineIndex], ',');
            /*trimLeadingNEndingWhitespaceFromStr(token);*/
            strcpy(newLexTree->content.directionSentence.content.dataDirection[counter],token);
            counter++;
            resetInnerIndex(newLexTree, strlen(token) + ANOTHER_CELL);
        }
        else{
            strcpy(newLexTree->content.directionSentence.content.dataDirection[counter], &rawLine[rawLineIndex]);
            break;
        }
    }while(token != NULL);
}

void setStringDirectionContent(lexTree *newLexTree){
    const char* rawLine = newLexTree->rawLine->content;
    size_t rawLineIndex = newLexTree->rawLineInnerIndex;
    size_t firstNonWhitespaceIndex = findFirstNonWhitespaceIndex(&rawLine[rawLineIndex]);
    size_t firstFromLastNonWhitespaceIndex = findFirstNonWhitespaceIndexFromEnd(&rawLine[rawLineIndex]);
    resetInnerIndex(newLexTree, firstNonWhitespaceIndex);
    int counter = ZEROISE_COUNTER;
    assemblyStringValidation(newLexTree,
             firstNonWhitespaceIndex,
             firstFromLastNonWhitespaceIndex);
    for(; (counter + newLexTree->rawLineInnerIndex + ANOTHER_CELL) <
          (firstFromLastNonWhitespaceIndex + newLexTree->rawLineInnerIndex - LAST_CELL);
          counter++){
        newLexTree->content.directionSentence.content.stringContent[counter] =
                rawLine[counter + newLexTree->rawLineInnerIndex + ANOTHER_CELL];
    }
    newLexTree->content.directionSentence.content.stringContent[counter - LAST_CELL] = END_OF_STRING;
}

void assemblyStringValidation(lexTree *newLexTree, size_t firstQuotationMarks, size_t lastQuotationMarks){
    const char* rawLine = newLexTree->rawLine->content;
    if(rawLine[newLexTree->rawLineInnerIndex] != '"'){
        newLexTree->error = firstAllowedCharAfterStringDeclarationIsQuotationMarks;
        return;
    }
    if (rawLine[newLexTree->rawLineInnerIndex + lastQuotationMarks] != '"'){
        newLexTree->error = lastAllowedCharAfterStringDeclarationIsQuotationMarks;
        return;
    }
    if (firstQuotationMarks >= lastQuotationMarks){
        newLexTree->error = needTowQuotationMarksInStringDeclaration;
        return;
    }
}

void setOrderLexTreeContent(lexTree *newLexTree){
    setOpCode(newLexTree);
    setGroup(newLexTree);
    setOperands(newLexTree);
}

void setOperands(lexTree* newLexTree){
    int thisGroup = newLexTree->content.orderContent.group;
    if (thisGroup == undefinedGroup)
        setUndefinedGroup(newLexTree);
    if (thisGroup == firstGroup)
        setFirstGroup(newLexTree);
    if (thisGroup == secondGroup)
        setSecondGroup(newLexTree);
    if (thisGroup == thirdGroup)
        setThirdGroup(newLexTree);
}

void setUndefinedGroup(lexTree* newLexTree){
    newLexTree->error = undefinedCommand;
    newLexTree->content.orderContent.firstOperand[FIRST_INDEX] = END_OF_STRING;
    newLexTree->content.orderContent.secondOperand[FIRST_INDEX] = END_OF_STRING;
}

void setFirstGroup(lexTree* newLexTree){
    const char* relevantRawLine = &newLexTree->rawLine->content[newLexTree->rawLineInnerIndex];
    strcpy(newLexTree->content.orderContent.firstOperand, getTokensUpToChar(relevantRawLine, ','));
    resetInnerIndex(newLexTree,
                    strlen(newLexTree->content.orderContent.firstOperand) + ANOTHER_CELL + LAST_CELL);
    relevantRawLine = &newLexTree->rawLine->content[newLexTree->rawLineInnerIndex];
    strcpy(newLexTree->content.orderContent.secondOperand, relevantRawLine);
}

void setSecondGroup(lexTree* newLexTree){
    const char* relevantRawLine = &newLexTree->rawLine->content[newLexTree->rawLineInnerIndex];
    strcpy(newLexTree->content.orderContent.firstOperand, relevantRawLine);
    newLexTree->content.orderContent.secondOperand[FIRST_INDEX] = END_OF_STRING;
}

void setThirdGroup(lexTree *newLexTree){
    const char * relevantRawLine = &newLexTree->rawLine->content[newLexTree->rawLineInnerIndex];
    resetInnerIndex(newLexTree, ANOTHER_CELL);
    relevantRawLine = &newLexTree->rawLine->content[newLexTree->rawLineInnerIndex];
    strcpy(newLexTree->content.orderContent.firstOperand, relevantRawLine);
    newLexTree->content.orderContent.secondOperand[FIRST_INDEX] = END_OF_STRING;
}

void setGroup(lexTree* newLexTree){
    if(newLexTree->content.orderContent.opcode <= lea){
        newLexTree->content.orderContent.group = firstGroup;
        return;
    }
    if( newLexTree->content.orderContent.opcode && newLexTree->content.orderContent.opcode <= jsr){
        newLexTree->content.orderContent.group = secondGroup;
        return;
    }
    if( newLexTree->content.orderContent.opcode >= rts && newLexTree->content.orderContent.opcode <= hlt){
        newLexTree->content.orderContent.group = thirdGroup;
        return;
    }
    newLexTree->content.orderContent.group = undefinedGroup;
}

void setOpCode(lexTree* newLexTree){
    const char* relevantRawLine = &newLexTree->rawLine->content[newLexTree->rawLineInnerIndex];
    char* opCode;
    int opcodeCounter = ZEROISE_COUNTER;
    int iteration = sizeof (opcodeString) / sizeof (opcodeString[FIRST_INDEX]);
    int firstNonWhiteIndex = findFirstNonWhitespaceIndex(relevantRawLine);
    resetInnerIndex(newLexTree, firstNonWhiteIndex);
    relevantRawLine = &newLexTree->rawLine->content[newLexTree->rawLineInnerIndex];
    opCode = getTokenUpToWhiteSpace(relevantRawLine);
    resetInnerIndex(newLexTree, strlen(opCode) + findFirstNonWhitespaceIndex(&relevantRawLine[strlen(opCode)]));
    while(opcodeCounter < iteration){
        if (!strcmp(opCode, opcodeString[opcodeCounter])) {
            newLexTree->content.orderContent.opcode = opcodeCounter;
            return;
        }
        opcodeCounter++;
    }
    newLexTree->content.orderContent.opcode = opcodeCounter;
}

void setDefinitionLexTreeContent(lexTree *newLexTree){
    const char *relevantRawLine, *definitionName;
    int definitionNameStartingIndex, counter;
    resetInnerIndex(newLexTree, DEFINE_SENTENCE_IDENTIFIER_LENGTH);
    relevantRawLine = &newLexTree->rawLine->content[newLexTree->rawLineInnerIndex];
    definitionNameStartingIndex = findFirstNonWhitespaceIndex(relevantRawLine);
    definitionName = getTokensUpToChar(&relevantRawLine[definitionNameStartingIndex], '=');
    if(!isThereMandatoryWhiteSpace(relevantRawLine)){
        newLexTree->error = mandatoryWhiteCharAfterKeyWord;
        return;
    }
    resetInnerIndex(newLexTree, strlen(definitionName) + FIRST_CELL + LAST_CELL);
    strcpy(newLexTree->content.definitionContent.name, definitionName);
    for(counter = ZEROISE_COUNTER; counter <= strlen(newLexTree->rawLine->content); counter++){
        newLexTree->content.definitionContent.value[counter] =
                newLexTree->rawLine->content[newLexTree->rawLineInnerIndex + counter];
    }
    /*trimLeadingNEndingWhitespaceFromStr(newLexTree->content.definitionContent.value);
    trimLeadingNEndingWhitespaceFromStr(newLexTree->content.definitionContent.name);*/
}













