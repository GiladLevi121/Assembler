

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "SettingLexTree.h"
#include "lexTree.h"
#include "label.h"
#include "parser.h"

/*order match DirectionSentence struct in lexTree.h*/
const char* directionsTypeIdentifier[] = {".string",".data",
                                          ".entry", ".extern"};


const char* opcodeString[] =          {"mov", "cmp","add","sub",
                                       "lea","not","clr","inc",
                                       "dec","jmp","bne","red",
                                       "prn","jsr","rts","hlt"};

lexTree *lexTreeConstructor(const assemblyLineCode *inputAssemblyLine, int instructionCounter,
                            size_t currentlyDataImagePC, size_t currentlyCodeImagePC){
    lexTree *newLexTree = (lexTree*)malloc(sizeof (lexTree));
    newLexTree->InstructionCounter = instructionCounter;
    newLexTree->error = valid;
    if(inputAssemblyLine->status == lineOutOfBounds){
        newLexTree->error = lineLengthIsTooLong;
        return newLexTree;
    }
    newLexTree->potentialLabel = labelNodeConstructor(inputAssemblyLine);
    newLexTree->rawLine = inputAssemblyLine;
    newLexTree->rawLineInnerIndex = ZEROISE_COUNTER;
    setLexTreeType(newLexTree);
    setLexTreeContent(newLexTree);
    setLexTreeLabel(newLexTree, currentlyDataImagePC, currentlyCodeImagePC);
    return newLexTree;
}

void setLexTreeLabel(lexTree* newLexTree, size_t currentlyDataImagePC, size_t currentlyCodeImagePC){
    if(newLexTree->type == order)
        setLabelType(newLexTree->potentialLabel, false, currentlyDataImagePC,
                     currentlyCodeImagePC, newLexTree->InstructionCounter);
    else if(newLexTree->type == direction)
        setLabelType(newLexTree->potentialLabel, true, currentlyDataImagePC,
                     currentlyCodeImagePC, newLexTree->InstructionCounter);
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
    else {
        setDefinitionLexTreeContent(newLexTree);
    }
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
    const char *lineAfterDirection;
    char firstNonWhiteSpaceAfterDirectionChar;
    int firstNonWhitespaceIndex;
    if (newLexTree->potentialLabel == NULL){
        lineAfterDirection = &newLexTree->rawLine->content[newLexTree->rawLineInnerIndex];
        firstNonWhitespaceIndex = findFirstNonWhitespaceIndex(lineAfterDirection);
    }else{
        char* lineWithoutLabel = getTokenWithoutFirstWordAndFollowedWhiteSpaces(
                newLexTree->rawLine->content);
        lineAfterDirection = getTokenWithoutFirstWordAndFollowedWhiteSpaces(lineWithoutLabel);
        firstNonWhitespaceIndex = findFirstNonWhitespaceIndex(lineAfterDirection);
    }
    if(!isThereMandatoryWhiteSpace(lineAfterDirection) && lineAfterDirection[FIRST_INDEX] != END_OF_STRING)
        newLexTree->error = mandatoryWhiteCharAfterKeyWord;
    if(firstNonWhitespaceIndex != ALL_WHITE_SPACES){
        firstNonWhiteSpaceAfterDirectionChar = lineAfterDirection[firstNonWhitespaceIndex];
        if( firstNonWhiteSpaceAfterDirectionChar == ',')
            newLexTree->error = commaCantBeTheFirsCharAfterDirectionDeclaration;
    }
    if (firstNonWhitespaceIndex == ALL_WHITE_SPACES)
        newLexTree->error = missingArgumentAfterDirection;

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
    if(newLexTree->content.directionSentence.type == dataDirection)
        setDataDirectionContent(newLexTree);
    else if (newLexTree->content.directionSentence.type == stringDirection)
        setStringDirectionContent(newLexTree);
    else if(newLexTree->content.directionSentence.type == entryDirection ||
             newLexTree->content.directionSentence.type == externDirection)
        setEntryNExternContent(newLexTree);
    else
        newLexTree->error = undefinedDirection;
}

void setEntryNExternContent(lexTree * newLexTree){
    char* token;
    const char* relevantRawLine = newLexTree->rawLine->content;
    size_t rawLineIndex = newLexTree->rawLineInnerIndex;
    size_t firstNonWhitespaceIndex = findFirstNonWhitespaceIndex(&relevantRawLine[rawLineIndex]);
    resetInnerIndex(newLexTree, firstNonWhitespaceIndex);
    relevantRawLine = &newLexTree->rawLine->content[newLexTree->rawLineInnerIndex];
    token = trimLeadingNEndingWhitespace(relevantRawLine);
    if(newLexTree->content.directionSentence.type == externDirection)
        initNSetExternDeclaration(newLexTree, token);
    else
        initNSetEntryDeclaration(newLexTree, token);
}

void setDataDirectionContent(lexTree* newLexTree) {
    size_t tokensAmount = getDataArgumentAmount(newLexTree);
    size_t tokenLength;
    const char *rawLine = newLexTree->rawLine->content;
    char *token;
    int i;
    initDataDeclarationDoublePointer(newLexTree, tokensAmount);
    for (i = ZEROISE_COUNTER; i < tokensAmount; i++) {
        size_t rawLineIndex = newLexTree->rawLineInnerIndex;
        if (strchr(&rawLine[rawLineIndex], ',') != NULL) {
            token = getTokensUpToChar(&rawLine[rawLineIndex], ',');
            tokenLength = strlen(token) + PADDING_CELL_LEN;  /*PADDING_CELL = len(',')*/
            token = trimLeadingNEndingWhitespace(token);
            if (token[FIRST_INDEX] != END_OF_STRING)
                addTokenToDataDirection(newLexTree, token);
            resetInnerIndex(newLexTree, tokenLength);
            free(token);
        }else{
            token = trimLeadingNEndingWhitespace(&rawLine [rawLineIndex]);
            if (token[FIRST_INDEX] != END_OF_STRING)
                addTokenToDataDirection(newLexTree, token);
            break;
        }
    }
}

size_t getDataArgumentAmount(lexTree* newLexTree){
    const char* relevantRawLine = &newLexTree->rawLine->content[newLexTree->rawLineInnerIndex];
    size_t index = ZEROISE_COUNTER;
    size_t tokensAmount = ZEROISE_COUNTER;
    boolean foundToken = false;
    if(newLexTree->error != valid)
        return ZEROISE_COUNTER;

    while(relevantRawLine[index] != END_OF_STRING){
        if(!isspace(relevantRawLine[index]))/*Empty line check*/
            foundToken = true;
        if(relevantRawLine[index] == ',')
            tokensAmount++;
        index ++;
    }
    if(foundToken)
        tokensAmount++;
    return tokensAmount;
}

void setStringDirectionContent(lexTree *newLexTree){
    char* token;
    const char* relevantRawLine = newLexTree->rawLine->content;
    size_t rawLineIndex = newLexTree->rawLineInnerIndex;
    size_t firstNonWhitespaceIndex = findFirstNonWhitespaceIndex(&relevantRawLine[rawLineIndex]);
    resetInnerIndex(newLexTree, firstNonWhitespaceIndex);
    relevantRawLine = &newLexTree->rawLine->content[newLexTree->rawLineInnerIndex];
    token = trimLeadingNEndingWhitespace(relevantRawLine);
    initNSetStringDeclaration(newLexTree, token);
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
    newLexTree->content.orderContent.sourceOperand = NULL;
    newLexTree->content.orderContent.destinationOperand = NULL;
}

void setFirstGroup(lexTree* newLexTree){
    const char* relevantRawLine = &newLexTree->rawLine->content[newLexTree->rawLineInnerIndex];
    char *token = getTokensUpToChar(relevantRawLine, ',');
    const char *firstOperand, *secondOperand;
    if(token == NULL){
        newLexTree->content.orderContent.sourceOperand = NULL;
        newLexTree->content.orderContent.destinationOperand = NULL;
        return;
    }
    firstOperand = trimLeadingNEndingWhitespace(token);
    initNSetSourceOperand(newLexTree, firstOperand);
    resetInnerIndex(newLexTree, strlen(token) + PADDING_CELL_LEN);
    relevantRawLine = &newLexTree->rawLine->content[newLexTree->rawLineInnerIndex];
    secondOperand = trimLeadingNEndingWhitespace(relevantRawLine);
    initNSetDestinationOperand(newLexTree, secondOperand);
}

void setSecondGroup(lexTree* newLexTree){
    const char* relevantRawLine = &newLexTree->rawLine->content[newLexTree->rawLineInnerIndex];
    const char *firstOperand = trimLeadingNEndingWhitespace(relevantRawLine);
    initNSetDestinationOperand(newLexTree, firstOperand);
    newLexTree->content.orderContent.sourceOperand = NULL;
}

void setThirdGroup(lexTree *newLexTree){
    const char * relevantRawLine = &newLexTree->rawLine->content[newLexTree->rawLineInnerIndex];
    const char* axes = trimLeadingNEndingWhitespace(relevantRawLine);
    initNSetSourceOperand(newLexTree, axes);
    newLexTree->content.orderContent.destinationOperand = NULL;
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
    resetInnerIndex(newLexTree, strlen(opCode));
    while(opcodeCounter < iteration){
        if (!strcmp(opCode, opcodeString[opcodeCounter])) {
            newLexTree->content.orderContent.opcode = opcodeCounter;
            break;
        }
        opcodeCounter++;
    }
    newLexTree->content.orderContent.opcode = opcodeCounter;
}

void setDefinitionLexTreeContent(lexTree *newLexTree){
    setDefinitionName(newLexTree);
    setDefinitionValue(newLexTree);
}

void setDefinitionName(lexTree* newLexTree){
    const char *relevantRawLine, *definitionName;
    char* rawDefinitionName;
    int definitionNameStartingIndex;
    resetInnerIndex(newLexTree, DEFINE_SENTENCE_IDENTIFIER_LENGTH);
    relevantRawLine = &newLexTree->rawLine->content[newLexTree->rawLineInnerIndex];
    definitionNameStartingIndex = findFirstNonWhitespaceIndex(relevantRawLine);
    rawDefinitionName = getTokensUpToChar(&relevantRawLine[definitionNameStartingIndex], '=');
    if(!isThereMandatoryWhiteSpace(relevantRawLine)){
        newLexTree->error = mandatoryWhiteCharAfterKeyWord;
        initNSetDefinitionName(newLexTree, NULL);
        return;
    }
    if(rawDefinitionName == NULL){
        newLexTree->error = MissingEqualKnotInDefineSentence;
        initNSetDefinitionName(newLexTree, NULL);
        return;
    }
    definitionName = trimLeadingNEndingWhitespace(rawDefinitionName);
    initNSetDefinitionName(newLexTree, definitionName);
    resetInnerIndex(newLexTree, strlen(rawDefinitionName) + PADDING_CELL_LEN + definitionNameStartingIndex);
}

void setDefinitionValue(lexTree* newLexTree){
    const char *relevantRawLine, *definitionValue, * rawDefinitionValue;
    int definitionValueStartingIndex;
    if(newLexTree->content.definitionContent.name == NULL){
        initNSetDefinitionValue(newLexTree, NULL);
        return;
    }
    relevantRawLine = &newLexTree->rawLine->content[newLexTree->rawLineInnerIndex];
    definitionValueStartingIndex = findFirstNonWhitespaceIndex(relevantRawLine);
    resetInnerIndex(newLexTree, definitionValueStartingIndex);
    rawDefinitionValue = &newLexTree->rawLine->content[newLexTree->rawLineInnerIndex];
    definitionValue = trimLeadingNEndingWhitespace(rawDefinitionValue);
    initNSetDefinitionValue(newLexTree, definitionValue);
}
















