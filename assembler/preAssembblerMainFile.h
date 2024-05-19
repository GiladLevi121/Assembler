
#ifndef ASSEMBLER_PREASSEMBBLERMAINFILE_H
#define ASSEMBLER_PREASSEMBBLERMAINFILE_H

#include "line.h"
#include "macroUtiles.h"

#define STRING_END_OF_ROW "\n"
#define STRING_END_OF_STRING "\0"

#define CHARS_TO_WRITE_10_LINES (10 * 81 + 1) /* 10 lines * 81 chars (max) + 1 for '\0'*/

/* PreAssembler main function.*/
boolean runNReturnStatusOfPreAssembler(char *, macroList*);

/* "Cleaning"*/
void freeNRemoveUnnessaceryInCaseOfOpenFileFaile(char *, FILE *);

/* Return true if there is a problem with the input file, closes pointers if needed*/
boolean preAssemblyOpenFileCheckNReturnIsNeeded(FILE *, FILE *, char*);

void setAssemblyLineOfCodeStatusInPreAssemblerPass(assemblyLineCode* assemblyLine, macroList*);

/* Return true if empty line*/
boolean isEmptyLine(assemblyLineCode *);

/* Return true if first char is ';', if only white spaces before also return true*/
boolean isCommentLine(assemblyLineCode *);

/* Return true if a first word in the line in mcr*/
boolean isBeginningOfMacro(assemblyLineCode*);

/* Return true if a first word in the line in endmcr*/
boolean isEndingOfMacro(assemblyLineCode *);

/* Return true if macro used trim the axes white spaces before comparing*/
boolean isUsageOfAMacroFromList(const char* lineContent, macroList *thisList);

/* Replace macro name with its content*/
void replaceTheMacroNameWithMacroContentNWriteToFile(assemblyLineCode *, macroList *, FILE*);

/* Create a macro, add it to the list, set error if needed*/
void macroDetectedOperations(assemblyLineCode*, FILE*, boolean*, int*, const char *, macroList*);

/* return macro name, set error if detected*/
char* getMacroName(assemblyLineCode*, macroError*);

/* Printing error details*/
void printPreAssemblyError(assemblyLineCode*, int, const macroError*, const char*);

/* Return macro content, sets error if needed*/
char* getMacroContent(assemblyLineCode*, FILE*, boolean*, int*, const char*, macroError*);

/* Preform the assembly line operation needed in pre-assembly*/
void preAssemblyEveryLineOperations(assemblyLineCode*, FILE *,FILE* , int *,
                                    const char *, boolean *, macroList*);

#endif
