#include <stdio.h>

#include "globaldefinitionsNStructures.h"
#include "firstPassMainFile.h"
#include "label.h"
#include "memoryImage.h"
#include "entryExternDeclaration.h"
#include "secondPassMainFile.h"


int main(int argc, char *argv[]) {
    printf("sdf");
    memoryImage *fileMemoryImage;
    labelOrDefinitionList *openingLabelList;
    entryExternList* entryNExternDeclarationList;

    fileMemoryImage = memoryImageConstructor();
    openingLabelList = labelOrDefinitionListConstructor();
    entryNExternDeclarationList = entryExternListConstructor();

    runFirstPass(argv[1], openingLabelList, entryNExternDeclarationList, fileMemoryImage);



    return 0;
}
