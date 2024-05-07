

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
#define BEGINNING_ASSEMBLY_ARRAY_INDEX 0
#define END_OF_STRING '\0'
#define ANOTHER_CELL 1
#define SECOND_CELL_INDEX 1
#define LAST_CELL 1
#define TWO_WORDS 2
#define REG_NAME_LENGTH 3



#define REGISTERS_AMOUNT 8

#define DECIMAL 10
#define MEMORY_WORD_LENGTH_IN_CODE_IMAGE 12
#define IMAGE_WORD_IN_MEMORY_LENGTH 14

/*in .c file*/
#define REGISTERS_FIRST_INDEX_IN_KEY_WORDS 21

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
#define END_OF_ROW '\n'

extern const char* reservedAssemblyWords[];
/*------------------------------structs------------------------------*/

typedef enum {
    false = FALSE,/* = 0*/
    true
}boolean;


typedef enum {
    valid = 0,
    firstLetterOfDefinitionOrLabelTitleMustBeAlphabetic,
    labelOrDefinitionTitleTooLong,
    titleOfLabelOrDefinitionCantBeAKeyWord,
    notAllCharactersAreAlphabeticOrNumbers,
    mandatoryWhiteCharAfterKeyWord,
    undefinedCommand,
    missingArgument, /*missing string, data or label (to entry or extern)*/
    undefinedDirection,/*printf("Error: undefined direction: %s.", .....) */
    missingArgumentAfterDirection,

    /* Order sentence errors*/
    sourceOperandAddressMethodError,
    destinationOperandAddressMethodError,
    missingOperand,
    inCompatibleOperand,
    toMannyOperands,

    incompatibleArgumentForDataDeclaration,/*using when want to check if .data arguments are numbers*/

    invalidNameForEntryOrExternDeclaration,
    labelTitleAlreadyUsed,
    illegalDefinitionTitle,
    definitionCantHaveALabel,
    commaIsTheLastChar,
    commaCantBeTheFirsCharAfterDirectionDeclaration,
    missingQuotationMarksInStringDeclaration,

    illegalNumber,
    definitionNamingIsIllegal,
    MissingEqualKnotInDefineSentence,

    twoConsecutiveCommasFound,
    labelUsedIsNotDeclared,/*label used but not connected to any memory*/
    lineLengthIsTooLong
} errorType;



/*returns true if reserved word. false if not*/
boolean isAReservedWord(const char*);

/*Return true if str is one of the registers name*/
boolean isARegisterName(const char *);


void appendToken(char* , const char* );
#endif
