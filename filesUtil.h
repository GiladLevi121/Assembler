

#ifndef ASSEMBLER_FILESUTIL_H
#define ASSEMBLER_FILESUTIL_H

#include "globaldefinitionsNStructures.h"
#include "line.h"

#define LEN_OF_FILE_TYPE 3
#define FIRST_FILE 1
#define NOT_VALID_LINE 1
#define VALID_LINE 0
#define START_OF_NOTE ";"
#define AM_ENDING ".am"
#define AS_ENDING ".as"
#define OB_ENDING ".ob"
#define ENT_ENDING ".ent"





/*this function gets file name (string) and file ending (string)
 * and permissions access to the file, connect the name and returns
 * a FILE pointer with the if found this file in the current
 * directory, or null if file don't exist. user has to close the file*/
FILE *openFileByName(char *, char *, char *);

/*this function gets file name (string) and file ending (string)
 * connect them together and return the connected string.
 * User has to free the file name*/
char *connectFileNameWithEnding(char *, char *);

assemblyLineCode *getNextAssemblyLine(FILE *);

#endif
