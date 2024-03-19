//
// Created by Levi Gila'ad on 01/03/2024.
//

#ifndef ASSEMBLER_FIRSTPASSMAINFILE_H
#define ASSEMBLER_FIRSTPASSMAINFILE_H

#include "label.h"
#include "lexTree.h"

/*this function activates first pass on a file. it gets
 *file's name (without ending),  */
void runFirstPass(char *, labelOrDefinitionList*, labelOrDefinitionList* );

void listsUpdating(labelOrDefinitionList*, lexTree*);

#endif //ASSEMBLER_FIRSTPASSMAINFILE_H
