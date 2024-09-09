#ifndef STRAND_H
#define STRAND_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

// Node struct: stores x, y coordinates
// useful for the 'neighbor' feature in code
typedef struct {
    int x;
    int y;
}Node;

// path struct: stores xPos, yPos, remaining, word, neighbors, num_neigbors
// useful to represent a new path created in the DFS function to store all information in one place
typedef struct {
    int x;
    int y;
    int remaining;
    char* word;
    Node* neighbors;
    int num_neighbors;
}Path;


// REQUIRED STRUCT - DO NOT REMOVE STRUCT MEMBERS
typedef struct strand
{
    char** words;
    char** letters;
    int rows, cols;
    int numWords;
    int wordLen;
    char** correct;
    int correct_num;

} strand_t;

// REQUIRED FUNCTIONS
strand_t * create_strand(FILE * dataFP, FILE * dictFP);
bool is_word_in_strand(strand_t * strand, char * word);
void print_strand_matrix(strand_t * strand);
void free_strand(strand_t * strand);

// my helper functions
char** read_dict(FILE* dictFP, int* dLen);
void check_duplicates(char** ans, char* word, int *sizePtr);
int binary_search_dict(char** dict, char* word, int* dLen);
void add_neigbors(Path* path, int x1, int y1);
void find_neighbors(Path* path, int rows, int cols);
void recur_find_words(strand_t* strand, Path* path, char** dict, char** ans, int *sizePtr, int* dLen);
char** iter_find_words(strand_t* strand, int* sizePtr, FILE* dict);
bool already_guessed(strand_t* strand, char* word);


#endif