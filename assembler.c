#include <stdio.h>

#include "globaldefinitionsNStructures.h"
#include "firstPassMainFile.h"
#include "label.h"
#include "memoryImage.h"


int main(int argc, char *argv[]) {
    int i = 0;
    memoryImage *fileMemoryImage = memoryImageConstructor();
    labelOrDefinitionList *openingLabelList = labelOrDefinitionListConstructor();
    labelOrDefinitionList *entryNExternalList = labelOrDefinitionListConstructor();
    runFirstPass(argv[1], openingLabelList, entryNExternalList, fileMemoryImage);
    deallocateLabelListElements(openingLabelList);
    freeMemoryImage(fileMemoryImage);
    return 0;
}
