
#ifndef ASSEMBLER_LABELNDEFINEVALIDATION_H
#define ASSEMBLER_LABELNDEFINEVALIDATION_H

#include <string.h>

#include "globaldefinitionsNStructures.h"

#define TITLE_MAX_LENGTH 31
#define LABEL_MAX_LENGTH_WITH_PADDING_CELL 32
#define NO_LABEL '\0'
#define LABEL_PADDING 0
#define DEFINED_IN_ASSEMBLY_PADDING 4




/*gets title, and title type (label/define) and returns errorType*/
errorType isNotLegalTitle(const char*);

/*return true if all chars are allowed chars for label/defined number.
 *false if not*/
boolean isTitleCharsAreAllowed(const char*);

#endif
