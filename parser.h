

#ifndef ASSEMBLER_PARSER_H
#define ASSEMBLER_PARSER_H

#include "globaldefinitionsNStructures.h"

#define ALL_WHITE_SPACES (-1)


int findFirstNonWhitespaceIndexFromEnd(const char*);

int findFirstNonWhitespaceIndex(const char*);

/*return sub-string up to char, or null if not exist*/
char * getTokensUpToChar(const char*, char);

/*returns true if there are 2 consecutive commas (ignore white spaces)
 * false if not*/
boolean twoConsecutiveCommas(const char *str);

/*returns true if comma is the last char (ignore white spaces at the end)
 * false if not*/
boolean endsWithComma(const char *);

/*return true if the first char in the string is space or /t.
 * return NULL if string is empty*/
boolean isThereMandatoryWhiteSpace(const char*);

/*return token up to white space. null if there isn't white space*/
char* getTokenUpToWhiteSpace(const char*);

/*return true if string is legal number according to the instructions.
 * ignoring beginning n ending white spaces. gets min and max rang,
 * if the number isn't in the rang or not a number, returns false.*/
boolean isLegalXBitsNumber(const char*, int , int );


boolean is14BitsLegalNumber(const char*);

boolean is12BitsLegalNumber(const char*);

void trimLeadingNEndingWhitespaceFromStr(char *);
#endif
