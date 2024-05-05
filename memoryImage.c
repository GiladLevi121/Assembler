
#include <stdlib.h>
#include <stdio.h>
#include "string.h"

#include "globaldefinitionsNStructures.h"
#include "memoryImage.h"

memoryImage *memoryImageConstructor(){
    memoryImage * newMemoryImage = (memoryImage*)malloc(sizeof (memoryImage));
    newMemoryImage->PC = FIRST_FREE_WORD;

    newMemoryImage->dataImage = (char**) malloc(sizeof (char*));
    return newMemoryImage;
}

void setFirstWordInCodeImage(memoryImage * thisMemoryImage, const char* word){
    if(strlen(word) != IMAGE_WORD_IN_MEMORY_LENGTH)
        printf("ERROR IN CODDING (setFirstWordInCodeImage) MUST CHECK");
    thisMemoryImage->codeImage = (char**) malloc(sizeof (char*));
    thisMemoryImage->codeImage[FIRST_INDEX] = (char*) malloc(
            IMAGE_WORD_IN_MEMORY_LENGTH * sizeof (char));
    strcpy(thisMemoryImage->codeImage[FIRST_INDEX], word);
    thisMemoryImage->currentlyWordsInCodeImage = FIRS_WORD_OCCUPIED;
}

void addToCodeImage(memoryImage* thisMemoryImage, const char** words){
    size_t wordsAmount = sizeof (words) / sizeof (words[FIRST_INDEX]);
    int counter = ZEROISE_COUNTER;
    size_t newMemoryWordsAmount =
            thisMemoryImage->currentlyWordsInCodeImage + wordsAmount;
    if(thisMemoryImage->codeImage != NULL){
        thisMemoryImage->codeImage = (char**) realloc(thisMemoryImage->codeImage,
                                                      newMemoryWordsAmount * sizeof (char*));
    }
    for(; counter < wordsAmount; counter++){
        strcpy(thisMemoryImage->codeImage[thisMemoryImage->currentlyWordsInCodeImage + counter + ANOTHER_CELL],
               words[counter]);
    }
    thisMemoryImage->currentlyWordsInCodeImage = newMemoryWordsAmount;
}

/*void increaseCodeImage*/