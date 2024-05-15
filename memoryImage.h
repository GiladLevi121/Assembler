

#ifndef ASSEMBLER_MEMORYIMAGE_H
#define ASSEMBLER_MEMORYIMAGE_H

#include <stdlib.h>
#include <stdio.h>

#define FIRST_WORD_INDEX 0
#define FIRS_WORD_OCCUPIED 1
#define ONE_WORD_TO_CODE 1
#define SECOND_WORD_INDEX 1
#define TWO_WORD_TO_CODE 2
#define THIRD_WORD_INDEX 2
#define THREE_WORD_TO_CODE 3
#define FOURTH_WORD_INDEX 3
#define FOUR_WORD_TO_CODE 4
#define FIFTH_WORD_INDEX 4
#define FIVE_WORD_TO_CODE 5
#define FIRST_FREE_WORD 100
#define BLOCK_OF_MEMORY 128


typedef struct {
    size_t PC;
    size_t currentlyWordsInCodeImage;
    size_t currentlyWordsInDataImage;
    size_t currentlyAllocatedWordsCodeImage;
    size_t currentlyAllocatedWordsDataImage;
    char **codeImage;
    char **dataImage;
}memoryImage;

/* Construct memoryImage: allocating and assigned initiate values*/
memoryImage *memoryImageConstructor();

/* Reallocating memory and add the char** to codeImage*/
void addToCodeImage(memoryImage*, char**, size_t);

/* Reallocating memory and add the char** to dataImage*/
void addToDataImage(memoryImage*, char** , size_t);

/*freeing memoryImage*/
void freeMemoryImage(memoryImage*);
#endif
