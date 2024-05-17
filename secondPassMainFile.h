
#ifndef ASSEMBLER_SECONDPASSMAINFILE_H
#define ASSEMBLER_SECONDPASSMAINFILE_H

#include "memoryImage.h"
#include "globaldefinitionsNStructures.h"
#include "label.h"
#include "filesUtil.h"
#include "lexTree.h"
#include "entryExternDeclaration.h"

#define LAST_LINE_TO_ERASE 1
#define ANOTHER_LINE 1
#define OBJ_LINE_LENGTH_WITH_PADDING 14
#define FIRST_LINE_MAX_LENGTH 20
#define NUMBER_TO_REPRESENT_4_DIGITS_NUMBER 1000

/* Main function of second pass*/
void runSecondPass(char *, /*boolean errorExist,*/
                   labelOrDefinitionList* ,
                   entryExternList * , memoryImage* );

/* Main operations that every line of assembly code need to be done,
 * in the second pass*/
void secondPassEveryLineOfAssemblyOperations(assemblyLineCode *, int, labelOrDefinitionList*,
                                             entryExternList *, memoryImage*);

/* Check for second pass errors*/
void additionValidationSecondPass(lexTree *,labelOrDefinitionList *,entryExternList*);

/* Main function for second pass coding */
void secondPassCoding(lexTree *, labelOrDefinitionList *, entryExternList *, memoryImage*);

/* Creates the output files*/
void createOutputFiles(memoryImage *, entryExternList *, const char*);

/* Creates the output externNEntryFile*/
void createEntNExtFile(entryExternList *, const char*);

/* Creates the output obj file*/
void createObjFile(memoryImage*, const char*);

/* Connect and return the line number + encrypted line as a string*/
char* getFormattedOBJLine(const char*, int);

/*write the first line to Obj file*/
void writeFirstLineToObjFile(FILE*, memoryImage*);

/* Prints the code image to .ob file*/
void printCodeImageToFile(FILE*, memoryImage*);

/* Prints the data image to .ob file*/
void printDataImageToFile(FILE*, memoryImage*);

/* Prints to .ext file*/
void printLineToeExtFile(entryExternNode* thisNode, FILE *extFilePointer);

/* Prints to .ent file*/
void printLineToEntFile(entryExternNode* thisNode, FILE *entFilePointer);

/* Closes .ent and .ext if needed*/
void closeFilesIfNeed(FILE *, FILE *, boolean, boolean );
#endif
