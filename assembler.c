#include <stdio.h>

#include "globaldefinitionsNStructures.h"
#include "firstPassMainFile.h"
#include "label.h"
#include "memoryImage.h"
#include "entryExternDeclaration.h"
#include "secondPassMainFile.h"


int main(int argc, char *argv[]) {
    //int i = 0;
    memoryImage *fileMemoryImage = memoryImageConstructor();
    labelOrDefinitionList *openingLabelList = labelOrDefinitionListConstructor();
    entryExternList* entryNExternDeclarationList = entryExternListConstructor();

    runFirstPass(argv[1], openingLabelList, entryNExternDeclarationList, fileMemoryImage);
    printf("blabla");
    runSecondPass(argv[1], openingLabelList, entryNExternDeclarationList, fileMemoryImage);
    printf("blabla");
    deallocatingEntryExternList(entryNExternDeclarationList);
    deallocateLabelListElements(openingLabelList);
    freeMemoryImage(fileMemoryImage);

    return 0;
}
