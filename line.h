
#ifndef ASSEMBLER_LINE_H
#define ASSEMBLER_LINE_H
#include "globaldefinitionsNStructures.h"



/*------------------------------structs------------------------------*/

typedef enum {
    validLine,
    lineOutOfBounds,
    emptyLine /* \n, spaces or \t, shorter then 80 chars.*/
}lineStatus;

typedef struct {
    lineStatus status;
    char content[MAX_CHARS_IN_LINE];
} assemblyLineCode;

/*------------------------------functions------------------------------*/

/*sets empty assemblyLineCode instance*/
assemblyLineCode *assemblyLineCodeConstructor(void);

/*set assemblyLineCode correspondingly to a raw line*/
void setAssemblyLineCode(assemblyLineCode*, const char*);

/*set assemblyLineCode content correspondingly to a raw line.
 * sets assemblyLineCode->content[lastChar] = '\0'.*/
void setContent(assemblyLineCode*, const char* rawLine);

/*this function gets assemblyLineCode that its content is set,
 *and sets its status correspondingly*/
void setStatus(assemblyLineCode*);

/*this function gets a raw line from file and returns its status.
 * this function assumed that there is '\0' at the end of the line's
 * content*/
lineStatus getContentStatus(assemblyLineCode *);

#endif
