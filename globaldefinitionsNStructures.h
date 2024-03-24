

#ifndef ASSEMBLER_GLOBALDEFINITIONSNSTRUCTURES_H
#define ASSEMBLER_GLOBALDEFINITIONSNSTRUCTURES_H

#define MEMORY_WORD_LENGTH 14
#define WORDS_IN_MEMORY 4096
#define FIRST_FILE 1
#define EQUAL_STRINGS 0
#define FALSE 0
#define FIRST_INDEX 0
#define ZEROISE_COUNTER 0
#define ZERO_CHARACTERS 0
#define END_OF_STRING '\0'
#define ANOTHER_CELL 1
#define FIRST_CELL 1
#define LAST_CELL 1

#define CHARS_TO_REPRESENT_LINE_IN_MEMORY 4
#define ORDER_MAX_WORD_IN_MEMORY 4 /*5,: 0 to 4 = 5*/
#define MAX_OPCODE_LENGTH 3

#define DECIMAL 10


#define MAX_NUMBERS_IN_DATA_DECLARATION 35 /*36 actually but starts from index 0 therefor 35*/
#define MAX_NUMBER_LENGTH_IN_DEFINE 72
#define MAX_STRING_LENGTH 73
#define MAX_LABEL_LENGTH_AFTER_ENTRY 74
#define MAX_LABEL_LENGTH_AFTER_EXTERN 73
#define MAX_NUMBER_LENGTH 74
#define INPUT_LINE_LENGTH 80
#define LINE_LENGTH_WITH_N 81
#define MAX_CHARS_IN_LINE 82

#define TWELVE_BITS_MAX_NUMBER 2047
#define TWELVE_BITS_MIN_NUMBER (-2048)

#define FOURTEEN_BITS_MAX_NUMBER 8191
#define FOURTEEN_BITS_MIN_NUMBER (-8192)

#define DIRECTION_OR_DEFINITION_SENTENCE_KNOT '.'



/*------------------------------structs------------------------------*/

typedef enum {
    false = FALSE,/* = 0*/
    true
}boolean;


typedef enum {
    valid,
    firstLetterOfDefinitionOrLabelTitleMustBeAlphabetic,
    labelOrDefinitionTitleTooLong,
    titleOfLabelOrDefinitionCantBeAKeyWord,
    notAllCharactersAreAlphabeticOrNumbers,
    mandatoryWhiteCharAfterKeyWord,
    undefinedCommand,
    missingArgument, /*missing string, data or label (to entry or extern)*/
    undefinedDirection,/*printf("Error: undefined direction: %s.", .....) */
    argumentIsNotANumber,/*using when want to check if .data arguments are numbers*/
    invalidEntryLabel,
    invalidExternLabel,
    labelTitleAlreadyUsed,
    illegalDefinitionTitle,
    definitionCantHaveALabel,
    commaIsTheLastChar,
    commaCantBeTheFirsCharAfterDirectionDeclaration,
    firstAllowedCharAfterStringDeclarationIsQuotationMarks,
    lastAllowedCharAfterStringDeclarationIsQuotationMarks,
    needTowQuotationMarksInStringDeclaration,
    illegalNumber,
    definitionNamingIsIllegal,
    misiingEqualKnotInDefineSentence,
    inCompatibleOperand,
    missingOperand,
    toMannyOperands,
    twoConsecutiveCommasFound,
    labelUsedIsNotDeclared,/*label used but not connected to any memory*/
    lineLengthIsTooLong
} errorType;



/*returns true if reserved word. false if not*/
boolean isAReservedWord(const char*);



#endif
