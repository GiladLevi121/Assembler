
#ifndef ASSEMBLER_MACROUTILES_H
#define ASSEMBLER_MACROUTILES_H

#include "line.h"

typedef enum {
    addingCharToMacro,
    regularLine,
    emptyLine,
}previousLineStatus;



typedef struct {
    char* name;
    char* content;
    struct macro* next;
}macro;

/* Construct macro*/
macro * macroConstructor(char*, char*);

#endif
