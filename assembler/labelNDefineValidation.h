
#ifndef ASSEMBLER_LABELNDEFINEVALIDATION_H
#define ASSEMBLER_LABELNDEFINEVALIDATION_H

#include <string.h>

#include "globaldefinitionsNStructures.h"
#include "parser.h"

#define TITLE_MAX_LENGTH 31
#define LABEL_MAX_LENGTH_WITH_PADDING_CELL 32





/*gets title, and title type (label/define) and returns errorType*/
errorType isNotLegalTitle(const char*);

/* Same as "isNotLegalTitle" function, only ignore beginning and closing white spaces*/
errorType isNotLegalTitleWithoutBeginningNEndingWhiteSpaces(const char*);

/* Return true if all chars are allowed chars for label/defined number.
 * false if not*/
boolean isTitleCharsAreAllowed(const char*);

#endif
