

#ifndef ASSEMBLER_MEMORYIMAGE_H
#define ASSEMBLER_MEMORYIMAGE_H


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


typedef struct {
    size_t PC;
    size_t currentlyWordsInCodeImage;
    size_t currentlyWordsInDataImage;
    char **codeImage;
    char **dataImage;
}memoryImage;

/* Construct memoryImage*/
memoryImage *memoryImageConstructor();

/* Allocating memory*/
void setFirstInstructionInCodeImage(memoryImage *thisMemoryImage, const char *word);

/* Reallocating memory and add the const char** to codeImage*/
void addToCodeImage(memoryImage*, const char**);
#endif
