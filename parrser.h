

#ifndef ASSEMBLER_PARRSER_H
#define ASSEMBLER_PARRSER_H

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

void trimLeadingNEndingWhitespaceFromStr(char *);
#endif
