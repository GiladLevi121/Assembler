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
    for (; i < fileMemoryImage->currentlyWordsInCodeImage; i++) {
        if(fileMemoryImage->codeImage[i] != NULL)
            printf("%s \n", fileMemoryImage->codeImage[i]);
        else
            printf("NULL\n");
    }
    i = 0;
    for (; i < fileMemoryImage->currentlyWordsInDataImage; i++) {
        if(fileMemoryImage->dataImage[i] != NULL)
            printf("%s \n", fileMemoryImage->dataImage[i]);
        else
            printf("NULL\n");
    }
    freeMemoryImage(fileMemoryImage);
    return 0;
}
