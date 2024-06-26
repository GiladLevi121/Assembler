

#ifndef ASSEMBLER_GLOBALDEFINITIONSNSTRUCTURES_H
#define ASSEMBLER_GLOBALDEFINITIONSNSTRUCTURES_H


#define EQUAL_STRINGS 0
#define FALSE 0
#define FIRST_INDEX 0
#define ZEROISE_COUNTER 0
#define ZERO_CHARACTERS 0
#define BEGINNING_ASSEMBLY_ARRAY_INDEX 0
#define EMPTY 0
#define END_OF_STRING '\0'
#define FILE_NUMBER_ONE 1
#define PADDING_CELL_LEN 1
#define SECOND_CELL_INDEX 1
#define MEMORY_INCREMENT_FACTOR 2
#define TWO_WORDS 2
#define REG_NAME_LENGTH 3
#define CHARS_NEEDED_TO_REPRESENT_LAST_MEMORY_LINE 5



#define REGISTERS_AMOUNT 8

#define DECIMAL 10
#define MEMORY_WORD_LENGTH_IN_CODE_IMAGE 12
#define IMAGE_WORD_IN_MEMORY_LENGTH 14

/*in .c file*/
#define REGISTERS_FIRST_INDEX_IN_KEY_WORDS 21

#define MAX_NUMBERS_IN_DATA_DECLARATION 35 /*36 actually but starts from index 0 therefor 35*/
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
extern const char* errorStings[];

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

    labelNameIsAlreadyUsedAsMacroName,
    invalidNameForEntryOrExternDeclaration,
    labelTitleAlreadyUsed,
    definitionCantHaveALabel,
    commaIsTheLastChar,
    commaCantBeTheFirsCharAfterDirectionDeclaration,
    missingQuotationMarksInStringDeclaration,

    forbiddenUseOfReservedWord,
    illegalNumber,
    definitionNamingIsIllegal,
    MissingEqualKnotInDefineSentence,

    declaredEntryLabelButDidntDeclaredLabelInFile,
    unKnownLabelUsage,
    cantUseExternalDeclarationNameToAlreadyExistLabelOrDefinition,
    entryOrExternDeclarationCantAppearTwice,
    cantUseEntryDeclarationToReferToDefinitionName,
    twoConsecutiveCommasFound,
    lineLengthIsTooLong
} errorType;



/*returns true if reserved word. false if not*/
boolean isAReservedWord(const char*);

/*Return true if str is one of the registers name*/
boolean isARegisterName(const char *);

/* Prints errors*/
void printErrorToUser(const char*, errorType, int);

#endif
