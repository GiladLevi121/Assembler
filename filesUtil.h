

#ifndef ASSEMBLER_FILESUTIL_H
#define ASSEMBLER_FILESUTIL_H

#include "globaldefinitionsNStructures.h"
#include "line.h"


#define START_OF_NOTE ";"
#define AM_ENDING ".am"
#define AS_ENDING ".as"
#define OB_ENDING ".ob"
#define ENT_ENDING ".ent"
#define EXT_ENDING ".ext"


/*this function gets file name (string) and file ending (string)
 * and permissions access to the file, connect the name and returns
 * a FILE pointer with the if found this file in the current
 * directory, or null if file don't exist. user has to close the file*/
FILE *openFileByName(const char *, char *, char *);

/*this function gets file name (string) and file ending (string)
 * connect them together and return the connected string.
 * User has to free the file name*/
char *connect2strings(const char *, char *);

assemblyLineCode *getNextAssemblyLine(FILE *);

/* Write line to a file*/
void writeStringToFile(FILE* filePointer, const char* line);

#endif
