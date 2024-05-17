

#ifndef ASSEMBLER_BASETRANSITIONUTILES_H
#define ASSEMBLER_BASETRANSITIONUTILES_H

#include "globaldefinitionsNStructures.h"

#define POSITIVE 0
#define ZEROISE_CARRY 0
#define CARRY_EXIST 1
#define SHIFT 1
#define CURRENT_BIT 1
#define ANOTHER_BIT 1
#define BINARY_MODULO 2

#define TWO_WORDS_AT_A_TIME 2
#define BASE_FOUR_WORD_LENGTH 7

/* Return 14 bit ascii value of char*/
char *charToBinaryString(char);

/* Convert an integer to its binary representation as a string
 * max length allowed is sizeof(int). Second int is for the string length
 * output*/
char *intToBinaryString(int , int);

/*return true <=> number is negative*/
boolean isNegative(int);

/*perform two's complement if negative number */
void complementToTwo(char *, int);

/* Convert memory word in binary to 4 base word, adds '\n' and after '\0'*/
char* memoryWordToEncrypted4Base(const char *);

/* Returns encrypted value of input char*/
char getEncryptedCharacter(char);
#endif
