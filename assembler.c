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
    entryExternList* entryNExternalList;

    fileMemoryImage = memoryImageConstructor();
    openingLabelNDefinitionList = labelOrDefinitionListConstructor();
    entryNExternalList = entryExternListConstructor();

    runFirstPass(argv[1], openingLabelNDefinitionList, entryNExternalList, fileMemoryImage);


    deallocatingEntryExternList(entryNExternalList);
    deallocateLabelListElements(openingLabelNDefinitionList);
    freeMemoryImage(fileMemoryImage);
    return 0;
}
