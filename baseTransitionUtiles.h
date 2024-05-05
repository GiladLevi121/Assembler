

#ifndef ASSEMBLER_BASETRANSITIONUTILES_H
#define ASSEMBLER_BASETRANSITIONUTILES_H

#include "globaldefinitionsNStructures.h"


#define ZEROISE_CARRY 0
#define CARRY_EXIST 1
#define SHIFT 1
#define CURRENT_BIT 1

/* Convert an integer to its binary representation as a string
 * max length allowed is sizeof(int). Second int is for the string length
 * output*/
char *intToBinaryString(int , int);

/*return true <=> number is negative*/
boolean isNegative(int);

/*perform two's complement if negative number */
void complementToTwo(char *, int);

#endif
