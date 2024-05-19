
#include <stdlib.h>

#include "globaldefinitionsNStructures.h"
#include "memoryImage.h"

memoryImage *memoryImageConstructor(){
    memoryImage * newMemoryImage = (memoryImage*)malloc(sizeof (memoryImage));
    newMemoryImage->PC = FIRST_FREE_WORD;
    newMemoryImage->secondPassCodeImageCounter = ZEROISE_COUNTER;
    newMemoryImage->dataImage = (char**) malloc(BLOCK_OF_MEMORY * sizeof (char*));
    newMemoryImage->codeImage = (char**) malloc(BLOCK_OF_MEMORY * sizeof (char*));
    newMemoryImage->currentlyAllocatedWordsCodeImage = BLOCK_OF_MEMORY;
    newMemoryImage->currentlyAllocatedWordsDataImage = BLOCK_OF_MEMORY;
    newMemoryImage->currentlyWordsInCodeImage = ZEROISE_COUNTER;
    newMemoryImage->currentlyWordsInDataImage = ZEROISE_COUNTER;
    return newMemoryImage;
}


void addToCodeImage(memoryImage* thisMemoryImage, char** words, size_t wordsAmount){
    int counter = ZEROISE_COUNTER;
    size_t overAllWords = thisMemoryImage->currentlyAllocatedWordsCodeImage + BLOCK_OF_MEMORY;
    if(thisMemoryImage->currentlyWordsInCodeImage + FIVE_WORD_TO_CODE >=
    thisMemoryImage->currentlyAllocatedWordsCodeImage){
        thisMemoryImage->codeImage = realloc(thisMemoryImage->codeImage,
                                             overAllWords * sizeof (char*));
    }
    for(; counter < wordsAmount; counter++){
        thisMemoryImage->codeImage[thisMemoryImage->currentlyWordsInCodeImage + counter] =
                words[counter];
    }
    thisMemoryImage->currentlyWordsInCodeImage += wordsAmount;
}

void addToDataImage(memoryImage* thisMemoryImage, char** words, size_t wordsAmount){
    int counter = ZEROISE_COUNTER;
    size_t overAllWords = thisMemoryImage->currentlyAllocatedWordsDataImage + BLOCK_OF_MEMORY;
    if(thisMemoryImage->currentlyWordsInDataImage + FIVE_WORD_TO_CODE >=
       thisMemoryImage->currentlyAllocatedWordsDataImage){
        thisMemoryImage->dataImage = realloc(thisMemoryImage->dataImage,
                                             overAllWords * sizeof (char*));
    }
    for(; counter < wordsAmount; counter++){
        thisMemoryImage->dataImage[thisMemoryImage->currentlyWordsInDataImage + counter] =
                words[counter];
    }
    thisMemoryImage->currentlyWordsInDataImage += wordsAmount;
}


void freeMemoryImage(memoryImage* thisMemoryImage){
    int i = ZEROISE_COUNTER;
    for (; i < thisMemoryImage->currentlyWordsInCodeImage; i++) {
        if(thisMemoryImage->codeImage[i] != NULL)
            free(thisMemoryImage->codeImage[i]);
    }
    for (; i < thisMemoryImage->currentlyWordsInDataImage; i++) {
        if(thisMemoryImage->dataImage[i] != NULL)
            free(thisMemoryImage->dataImage[i]);
    }
    free(thisMemoryImage->codeImage);
    free(thisMemoryImage->dataImage);
}




boolean isMemoryFull(memoryImage * thisMemoryImage){
    if (thisMemoryImage->currentlyWordsInCodeImage +
    thisMemoryImage->currentlyWordsInDataImage +
    FIRST_FREE_WORD > MEMORY_SIZE)
        return true;
    return false;
}




