#include <stdio.h>

#include "globaldefinitionsNStructures.h"
#include "firstPassMainFile.h"
#include "label.h"

int main(int argc, char *argv[]) {
    labelOrDefinitionList *openingLabelList = labelOrDefinitionListConstructor();
    labelOrDefinitionList *definitionList = labelOrDefinitionListConstructor();
    runFirstPass(argv[1], openingLabelList, definitionList);
    return 0;
}
