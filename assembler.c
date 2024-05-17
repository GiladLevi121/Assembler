#include <stdio.h>

#include "globaldefinitionsNStructures.h"
#include "firstPassMainFile.h"
#include "label.h"
#include "memoryImage.h"
#include "entryExternDeclaration.h"
#include "secondPassMainFile.h"


int main(int argc, char *argv[]) {

    memoryImage *fileMemoryImage;
    labelOrDefinitionList *openingLabelNDefinitionList;
    entryExternList* entryExternLabelList;

    fileMemoryImage = memoryImageConstructor();
    openingLabelNDefinitionList = labelOrDefinitionListConstructor();
    entryExternLabelList = entryExternListConstructor();

    runFirstPass(argv[1], openingLabelNDefinitionList, entryExternLabelList, fileMemoryImage);
    runSecondPass(argv[1], openingLabelNDefinitionList, entryExternLabelList, fileMemoryImage);


    deallocatingEntryExternList(entryExternLabelList);
    deallocateLabelListElements(openingLabelNDefinitionList);
    freeMemoryImage(fileMemoryImage);
    return 0;
}
