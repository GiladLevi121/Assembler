#include <stdio.h>

#include "globaldefinitionsNStructures.h"
#include "firstPassMainFile.h"
#include "label.h"
#include "memoryImage.h"
#include "entryExternDeclaration.h"
#include "secondPassMainFile.h"
#include "preAssembblerMainFile.h"


int main(int argc, char *argv[]) {

    memoryImage *fileMemoryImage;
    labelOrDefinitionList *openingLabelNDefinitionList;
    entryExternList* entryExternLabelList;
    macroList * thisMacrosList;
    boolean fileErrorDetected;
    boolean memoryFull;
    int i = FILE_NUMBER_ONE;
    for (; i < argc; i++) {
        fileErrorDetected = false;
        memoryFull = false;
        thisMacrosList = macroListConstructor();
        fileMemoryImage = memoryImageConstructor();
        openingLabelNDefinitionList = labelOrDefinitionListConstructor();
        entryExternLabelList = entryExternListConstructor();

        if (!runNReturnStatusOfPreAssembler(argv[i], thisMacrosList)){ /* enters if didn't find error*/
            runFirstPass(argv[i], openingLabelNDefinitionList, entryExternLabelList,
                         fileMemoryImage, thisMacrosList, &fileErrorDetected, &memoryFull);
            if(!memoryFull)/* enters if memory isn't full*/
                runSecondPass(argv[i], openingLabelNDefinitionList, entryExternLabelList,
                              fileMemoryImage, &fileErrorDetected);
        }
        printf("\n\n\n");
        deAllocateMacroList(thisMacrosList);
        deallocatingEntryExternList(entryExternLabelList);
        deallocateLabelListElements(openingLabelNDefinitionList);
        freeMemoryImage(fileMemoryImage);
    }
    return 0;
}
