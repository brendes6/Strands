#include "strand.h"

char** read_dict(FILE* dictFP, int* dLen){

    // read_dict  - reads dict file into an array which is returned
    // increments dLen as dict is read in for size of dict

    // initialize dLen and size ints
    *dLen = 0;
    int size = 1000;

    // initialize array of size size and temporary string
    char** arr = malloc(sizeof(char*)*size);
    char temp[50];

    // iterate through dictionary with fscanf
    while (fscanf(dictFP, "%s\n", temp)!=EOF){
        int l = strlen(temp);

        // check if array needs to be realloced based on size and dLen
        if ((*dLen)==size-1){
            size += 1000;
            char** tempArr = realloc(arr, size*sizeof(char*));
            if (tempArr==NULL){
                printf("Error allocating dict memory\n");
                return NULL;
            }
            arr = tempArr;
         }

        // malloc size for word in array
        arr[*dLen] = malloc(sizeof(char) * l+1);
        // copy dict string to array
        strcpy(arr[*dLen], temp);
        // increment dLen
        (*dLen)++;
    }
    
    return arr;
}

void check_duplicates(char** ans, char* word, int *sizePtr){
    // check_duplicates: check if word is already in ans array, if not add it
    // input: ans -  array of already found words, word - char* of a word, sizePtr - size pointer

    // check int - only set to 1 if word is found
    int check = 0;
    int l = strlen(word);
    // iterate through arr, check if word is in it, if so set check to 1
    for (int i=0; i<(*sizePtr); i++){
        if (strcmp(word, ans[i])==0) check = 1;

    }

    // if word is not found,
    if (check==0){
        // malloc size for word in array
        ans[(*sizePtr)] = malloc(sizeof(char)*l+1);
        // set final spot to null
        word[l] = '\0';
        // copy word to ans, increment size
        strcpy(ans[(*sizePtr)], word);
        (*sizePtr)++;
    }
}

int binary_search_dict(char** dict, char* word, int* dLen){
    // binary_search_dict - binary search function for searching for a word in dict
    // binary search is much faster for large data sets that are sorted (O(logn))

    // initialize left and right to start and end of dict
    int l = 0;
    int r = *dLen;
    // while left hasnt met right,
    while (l<=r){

        // binary search is hard to explain in comments, but this works
        int m = (l+r) / 2;
        int res = strcmp(word, dict[m]);
        if (res>0) {
            l = m+1;
        }
        if (res<0) {
            r = m-1;
        }
        // if word is found, return 1
        if (res==0) {
            return 1;
        }
    }
    // if this point is reached, word is not found, return 0
    return 0;
}

void add_neighbors(Path* path, int x1, int y1){
    // create node, set x and y values
    Node node;
    node.x = x1;
    node.y = y1;
    // append node to path neighbors
    path->neighbors[path->num_neighbors] = node;
    path->num_neighbors++;
}

void find_neighbors(Path* path, int rows, int cols){

    // for each potential neighboring position, check if it is in range, if so call add_neighbors with it.
    int xPos = path->x;
    int yPos = path->y;
    if (xPos>0){
        if (yPos>0){
            add_neighbors(path, xPos-1, yPos-1);
            add_neighbors(path, xPos, yPos-1);
        }
        if (yPos<cols-1){
            add_neighbors(path, xPos-1, yPos+1);
        }
        add_neighbors(path, xPos-1, yPos);
        }
    else{
        if (yPos>0){
            add_neighbors(path, xPos, yPos-1);
        }
    }
    if (xPos<rows-1){
        if (yPos<cols-1){
            add_neighbors(path, xPos, yPos+1);
            add_neighbors(path, xPos+1, yPos+1);
        }
        if (yPos>0){
            add_neighbors(path, xPos+1, yPos-1);
        }
        add_neighbors(path, xPos+1, yPos);
        }
    else{
        if (yPos<cols-1){
            add_neighbors(path, xPos, yPos+1);
        }
    }
}

void free_path(Path* path){
    // free all components of path
    free(path->word);
    free(path->neighbors);
    free(path);
}

void recur_find_words(strand_t* strand, Path* path, char** dict, char** ans, int *sizePtr, int* dLen){

    // if path has remaining of zero, the word is as long as it is supposed to be
    // check if it is in dictionary, also a duplicate, and then free path

    if (path->remaining==0){
        if (binary_search_dict(dict, path->word, dLen)==1) {
            check_duplicates(ans, path->word, sizePtr);
        }
        free_path(path);
        return;
    }

    // find_neighbors adds neighbor nodes to the path
    find_neighbors(path, strand->rows, strand->cols);
    for (int i=0; i<path->num_neighbors; i++){
        // for each neighbor, create a new path
        Path* new_path = (Path*)malloc(sizeof(Path));
        // new path has x and y values of neighbor
        new_path->x = path->neighbors[i].x;
        new_path->y = path->neighbors[i].y;
        // new path word gets malloced
        new_path->word = malloc(sizeof(char)*(strand->wordLen+1));
        new_path->remaining = path->remaining-1;
        new_path->num_neighbors = 0;
        new_path->neighbors = malloc(sizeof(Node)*8);
        // copy old path word to newpath word and add the letter att the specific position
        strcpy(new_path->word, path->word);
        new_path->word[strand->wordLen - path->remaining] = (strand->letters)[new_path->x][new_path->y];
        new_path->word[strand->wordLen - path->remaining + 1] = '\0';

        // call recursive function with new path
        recur_find_words(strand, new_path, dict, ans, sizePtr, dLen);

    }
    // free paths
    free_path(path);
    return;

}

char** iter_find_words(strand_t* strand, int* sizePtr, FILE* dictFP){
    // initialize dictionary length int and read in dict
    int d = 0;
    int* dLen = &d;
    char** dict = read_dict(dictFP, dLen);
    // initialize ans array that words will be added to
    char** ans = malloc(sizeof(char*)*200);

    // for each position in the strand matrix: 
    for (int i=0; i<(strand->rows); i++){
        for (int j=0; j<(strand->cols); j++){
            // create new 'path' struct for speciic position
            Path* path = (Path*)malloc(sizeof(Path));
            // add xPos, yPos, word, remaining and numNeighbor values
            path->x = i;
            path->y = j;
            path->word = malloc(sizeof(char)*(strand->wordLen+1));
            path->word[0] = (strand->letters)[i][j];
            path->word[1] = '\0';
            path->remaining = strand->wordLen - 1;
            path->num_neighbors = 0;
            // malloc space for path's neighbors
            path->neighbors = malloc(sizeof(Node)*8);
            // call recursive function with strand and new path
            recur_find_words(strand, path, dict, ans, sizePtr, dLen);
        }
    }
    // free all dict words
    for (int i=0; i<(*dLen); i++){
        free(dict[i]);
    }
    // free dict pointer
    free(dict);
    return ans;
}

// REQUIRED
strand_t * create_strand(FILE * dataFP, FILE * dictFP) {
    // create_strand - builds strand, builds a new strand from data file, finds words, initializes all other aspects

    // create strand pointer
    strand_t* strand = (strand_t*)malloc(sizeof(strand_t));
    
    // scan in strand rows, cols, and wordLen from data file
    fscanf(dataFP, "%d %d ", &strand->rows, &strand->cols);
    fscanf(dataFP, "%d ", &strand->wordLen);

    // malloc and read in strand letters from file given rows and cols numbers
    strand->letters = malloc(sizeof(char*) * strand->rows);
    for (int i=0; i<strand->rows; i++){
        strand->letters[i] = malloc(sizeof(char) * (strand->cols));
        for (int j=0; j<strand->cols; j++){
            fscanf(dataFP, " %c", &strand->letters[i][j]);
        }
    }

    // initialize size pointer, pass strand and pointer into function that finds words
    int size = 0;
    int* sizePtr = &size;
    char** final_word_list = iter_find_words(strand, sizePtr, dictFP);

    // malloc space for correct and wordlist pointers
    strand->words = (char**)malloc(sizeof(char*) * (*sizePtr));
    strand->correct = (char**)malloc(sizeof(char*) * (*sizePtr));

    // map all found words in final_word_list to the strand words, free final_word_list
    // also malloc strand words and correct pointers based on size
    for (int i=0; i<(*sizePtr); i++){
        strand->words[i] = (char*)malloc(sizeof(char)*strand->wordLen + 1);
        strand->correct[i] = (char*)malloc(sizeof(char)*strand->wordLen + 1);
        strcpy(strand->words[i], final_word_list[i]);
        free(final_word_list[i]);
    }
    free(final_word_list);
    strand->numWords = (*sizePtr);
    strand->correct_num = 0;

    return strand;
}

// REQUIRED
bool is_word_in_strand(strand_t * strand, char * word) {
    // is_word_in_strand - returns if passed in word is in the strand
    // iterates through strand words, if in word return true, return false
    for (int i=0; i<strand->numWords; i++){
        if (strcmp(word, strand->words[i])==0) return true;
    }
    return false;
}

// REQUIRED
void print_strand_matrix(strand_t * strand) {
    // print_strand_matrix:
    // prints the specific matrix for a passed in strand.
    printf("\n");
    for (int i=0; i<strand->rows; i++){
        printf("\t");
        for (int j=0; j<strand->cols; j++){
            printf("+---");
        }
        printf("+\n");
        printf("\t");
        for (int j=0; j<strand->cols; j++){
            printf("| %c ", strand->letters[i][j]);
        }
        printf("|\n");
    }
    printf("\t");
    for (int j=0; j<strand->cols; j++){
        printf("+---");
    }
    printf("+\n");
}

bool already_guessed(strand_t* strand, char* word){
    // already_guessed - iterates through correctly guessed words, returns whether or not passed in word is in list
    for (int i=0; i<strand->correct_num; i++){
        if (strcmp(word, strand->correct[i])==0) return true;
    }
    return false;
}

// REQUIRED
void free_strand(strand_t * strand) {
    // free_strand - frees all memory allocated for strand

    // frees individual strand words and correctly guessed words
    for (int i=0; i<strand->numWords; i++){
        free(strand->words[i]);
        free(strand->correct[i]);
    }
    // frees pointer to words list
    free(strand->words);
    // frees each list of letters
    for (int i=0; i<strand->rows; i++){
        free(strand->letters[i]);
    }
    // frees pointer to rows
    free(strand->letters);
    // frees pointer to crrect list
    free(strand->correct);
    // frees strand pointer
    free(strand);
}