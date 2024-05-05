

#ifndef ASSEMBLER_MEMORYIMAGE_H
#define ASSEMBLER_MEMORYIMAGE_H

#define FIRST_FREE_WORD 100
#define FIRS_WORD_OCCUPIED 1

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
void setFirstWordInCodeImage(memoryImage *, const char*);

/* Reallocating memory and add the const char** to codeImage*/
void addToCodeImage(memoryImage*, const char**);
#endif
