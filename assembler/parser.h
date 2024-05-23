

#ifndef ASSEMBLER_PARSER_H
#define ASSEMBLER_PARSER_H

#include "globaldefinitionsNStructures.h"

#define ALL_WHITE_SPACES (-1)


/* Return the token without the first word seperated with white space*/
char* getTokenWithoutFirstWordAndFollowedWhiteSpaces(const char* );

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

/* Return true if string is legal number according to the instructions.
 * Ignoring beginning & ending white spaces if last argument is true, else(boolean = false)
 * don't ignoring white spaces <=> " 1", " 1 ", "1 " are not a number. gets min and max rang,
 * if the number isn't in the rang or not a number, returns false.*/
boolean isLegalXBitsNumber(const char*, int , int, boolean );

/*using isLegalXBitsNumber*/
boolean is14BitsLegalNumberIgnoreWhiteSpaces(const char*);


/*using isLegalXBitsNumber*/
boolean is14BitsLegalNumberAsIs(const char*);

/*using isLegalXBitsNumber*/
boolean is12BitsLegalNumberAsIs(const char*);

/*gets const char* x and return char* = x without leading and ending whitespace */
char* trimLeadingNEndingWhitespace(const char*);

/* Returns true if empty or all white spaces else false*/
boolean isEmptyString(const char *str);
#endif
