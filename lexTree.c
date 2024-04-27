

#include <string.h>
#include <stdlib.h>

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
//TODO: Set labelType.pc
lexTree *lexTreeConstructor(const assemblyLineCode *inputAssemblyLine, int pc){
    lexTree *newLexTree = (lexTree*)malloc(sizeof (lexTree));
    newLexTree->InstructionCounter = pc;
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
    return newLexTree;
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
    if(!isThereMandatoryWhiteSpace(lineAfterDirection) && lineAfterDirection[FIRST_INDEX] != END_OF_STRING)
        newLexTree->error = mandatoryWhiteCharAfterKeyWord;
    if(firstNonWhitespaceIndex != ALL_WHITE_SPACES){
        firstNonWhiteSpaceAfterDirectionChar =  newLexTree->rawLine->content[firstNonWhitespaceIndex];
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
        strcpy(newLexTree->content.directionSentence.content.externLabel, token);
    else
        strcpy(newLexTree->content.directionSentence.content.entryLabel, token);
}

void setDataDirectionContent(lexTree* newLexTree){
    size_t counter = ZEROISE_COUNTER;
    size_t tokenLength;
    const char* rawLine = newLexTree->rawLine->content;
    char* token;
    do {
        size_t rawLineIndex = newLexTree->rawLineInnerIndex;
        if(strchr(&rawLine[rawLineIndex], ',') != NULL){
            token = getTokensUpToChar(&rawLine[rawLineIndex], ',');
            tokenLength = strlen(token) + ANOTHER_CELL; /* ANOTHER_CELL = len(',')*/
            token = trimLeadingNEndingWhitespace(token);
            strcpy(newLexTree->content.directionSentence.content.dataDirection[counter],token);
            counter++;
            resetInnerIndex(newLexTree, tokenLength);
        }
        else{
            token = trimLeadingNEndingWhitespace(&rawLine [rawLineIndex]);
            strcpy(newLexTree->content.directionSentence.content.dataDirection[counter], token);
            break;
        }
    }while(token != NULL);
}

void setStringDirectionContent(lexTree *newLexTree){
    char* token;
    const char* relevantRawLine = newLexTree->rawLine->content;
    size_t rawLineIndex = newLexTree->rawLineInnerIndex;
    size_t firstNonWhitespaceIndex = findFirstNonWhitespaceIndex(&relevantRawLine[rawLineIndex]);
    resetInnerIndex(newLexTree, firstNonWhitespaceIndex);
    relevantRawLine = &newLexTree->rawLine->content[newLexTree->rawLineInnerIndex];
    token = trimLeadingNEndingWhitespace(relevantRawLine);
    strcpy(newLexTree->content.directionSentence.content.stringContent, token);
}

void assemblyStringValidation(lexTree *newLexTree, size_t firstQuotationMarks, size_t lastQuotationMarks){
    const char* rawLine = &newLexTree->rawLine->content[newLexTree->rawLineInnerIndex];
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
    char *token = getTokensUpToChar(relevantRawLine, ',');
    char *firstOperand, *secondOperand;
    if(token == NULL){
        newLexTree->content.orderContent.firstOperand[FIRST_INDEX] = END_OF_STRING;
        newLexTree->content.orderContent.secondOperand[FIRST_INDEX] = END_OF_STRING;
        return;
    }
    firstOperand = trimLeadingNEndingWhitespace(token);
    strcpy(newLexTree->content.orderContent.firstOperand, firstOperand);
    resetInnerIndex(newLexTree,strlen(token) + LAST_CELL);
    relevantRawLine = &newLexTree->rawLine->content[newLexTree->rawLineInnerIndex];
    secondOperand = trimLeadingNEndingWhitespace(relevantRawLine);
    strcpy(newLexTree->content.orderContent.secondOperand, secondOperand);
}

void setSecondGroup(lexTree* newLexTree){
    const char* relevantRawLine = &newLexTree->rawLine->content[newLexTree->rawLineInnerIndex];
    char *firstOperand = trimLeadingNEndingWhitespace(relevantRawLine);
    strcpy(newLexTree->content.orderContent.firstOperand, firstOperand);
    newLexTree->content.orderContent.secondOperand[FIRST_INDEX] = END_OF_STRING;
}

void setThirdGroup(lexTree *newLexTree){
    const char * relevantRawLine = &newLexTree->rawLine->content[newLexTree->rawLineInnerIndex];
    char* axes = trimLeadingNEndingWhitespace(relevantRawLine);
    strcpy(newLexTree->content.orderContent.firstOperand, axes);
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
    int definitionNameStartingIndex, definitionNameEndIndex, charsToCopy;
    resetInnerIndex(newLexTree, DEFINE_SENTENCE_IDENTIFIER_LENGTH);
    relevantRawLine = &newLexTree->rawLine->content[newLexTree->rawLineInnerIndex];
    definitionNameStartingIndex = findFirstNonWhitespaceIndex(relevantRawLine);
    definitionName = getTokensUpToChar(&relevantRawLine[definitionNameStartingIndex], '=');
    if(!isThereMandatoryWhiteSpace(relevantRawLine)){
        newLexTree->error = mandatoryWhiteCharAfterKeyWord;
        return;
    }
    if(definitionName == NULL){
        newLexTree->error = misiingEqualKnotInDefineSentence;
        return;
    }
    definitionNameEndIndex = findFirstNonWhitespaceIndexFromEnd(definitionName);
    charsToCopy = definitionNameEndIndex + LAST_CELL;
    memcpy(newLexTree->content.definitionContent.name, definitionName, charsToCopy);
    newLexTree->content.definitionContent.name[charsToCopy] = END_OF_STRING;
    resetInnerIndex(newLexTree, strlen(definitionName) + LAST_CELL + definitionNameStartingIndex);
}

void setDefinitionValue(lexTree* newLexTree){
    const char *relevantRawLine, *definitionValue;
    int definitionValueStartingIndex, definitionValueEndIndex, charsToCopy;
    relevantRawLine = &newLexTree->rawLine->content[newLexTree->rawLineInnerIndex];
    definitionValueStartingIndex = findFirstNonWhitespaceIndex(relevantRawLine);
    definitionValueEndIndex = findFirstNonWhitespaceIndexFromEnd(relevantRawLine);
    definitionValue = &newLexTree->rawLine->content[newLexTree->rawLineInnerIndex];
    charsToCopy = definitionValueEndIndex + LAST_CELL - definitionValueStartingIndex;
    memcpy(newLexTree->content.definitionContent.value,
           &definitionValue[definitionValueStartingIndex],
           charsToCopy);
    newLexTree->content.definitionContent.value[charsToCopy] = END_OF_STRING;
}














