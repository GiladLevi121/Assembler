#include <stdio.h>

#include "globaldefinitionsNStructures.h"
#include "firstPassMainFile.h"
#include "label.h"


int main(int argc, char *argv[]) {

    labelOrDefinitionList *openingLabelList = labelOrDefinitionListConstructor();
    labelOrDefinitionList *entryNExternalList = labelOrDefinitionListConstructor();
    runFirstPass(argv[1], openingLabelList, entryNExternalList);
    return 0;
}
