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
    boolean fileErrorDetected;


    fileErrorDetected = false;
    macroList * thisMacrosList = macroListConstructor();
    fileMemoryImage = memoryImageConstructor();
    openingLabelNDefinitionList = labelOrDefinitionListConstructor();
    entryExternLabelList = entryExternListConstructor();

    if (!runNReturnStatusOfPreAssembler(argv[1], thisMacrosList)){ /* enters if didn't find error*/
        runFirstPass(argv[1], openingLabelNDefinitionList, entryExternLabelList, fileMemoryImage, thisMacrosList);
        runSecondPass(argv[1], openingLabelNDefinitionList, entryExternLabelList, fileMemoryImage);
    }

    deAllocateMacroList(thisMacrosList);
    deallocatingEntryExternList(entryExternLabelList);
    deallocateLabelListElements(openingLabelNDefinitionList);
    freeMemoryImage(fileMemoryImage);
    return 0;
}
