#include "game.h"

// REQUIRED
FILE * open_data_file(int argc, char** argv) {
    // open_data_file - opens data file either provided in command line or prompted

    // create file pointer
    FILE* fptr;
    // if provided in command line, open argument 2
    if (argc>=2) fptr = fopen(argv[1], "r");

    // else, prompt user for name, open input as file
    else {
        char name[100];
        printf("Provide strand data filename:\n");
        scanf(" %s", name);
        fptr = fopen(name, "r");
    }
    // return opened file pointer
    return fptr;
}

// REQUIRED
FILE * open_dict_file(int argc, char** argv) {
    // open_dict_file - opens either provided dictionary or default linux dictionary
    FILE* fptr;
    // if 3 arguments given open the 3rd argument
    if (argc==3) fptr = fopen(argv[2], "r");
    // otherwise open the linux dict path
    else {
        fptr = fopen(DICT_PATH, "r");
    }

    // return pointer
    if (fptr==NULL) {
        return NULL;
    }
    return fptr;
}

// REQUIRED - DO NOT EDIT
void print_instructions(strand_t * strand) {
    printf("\n  +-- Welcome to Strands! --+\n");
    printf("\nInstructions:\n");
    printf("\tFind all %d %d-letter words\n", strand->numWords, strand->wordLen);
    printf("\tEach word's letters must touch\n");
    printf("\tA letter can be repeated in a word\n");
    printf("\tType a word then press 'Enter'\n");
    printf("\tType 'quit' when done\n");
}
// REQUIRED
void play_game(strand_t * strand) {
    // play_game - runs the functionality of the game as you play it.

    // game runs while run==1
    int run = 1;
    // declare guessed word variable
    char guess[strand->wordLen];
    while (run==1){
        // prompt user to guess word, scan for input
        printf("\nGuess a word or 'quit':\n");
        scanf("%s", guess);
        // if user inputs quit, print quit message and break out of loop
        if (strcmp(guess, "quit")==0){
            printf("You found %d out of %d %d-letter words.\n", strand->correct_num, strand->numWords, strand->wordLen);
            printf("Thanks for playing!\n");
            run = 0;
            break;
        }
        // if word is in strand:
        if (is_word_in_strand(strand, guess)){
            // if word has already been guessed, tell user it had been guessed
            if (already_guessed(strand, guess)){
                printf("You have already guessed %s!\n", guess);
            }
            // else, print that the word is in the strand, add the word to the correct word list in the strand.
            else{
                printf("%s is a valid word in the strand!\n", guess);
                strcpy(strand->correct[strand->correct_num], guess);
                strand->correct_num += 1;
            }
        }
        // if word is not in strand, print that it is not in the strand
        else printf("%s is not a valid word in the strand.\n", guess);
        // if user has guessed numWords words correct, print win message and break out of loop
        if (strand->numWords == strand->correct_num){
            printf("You found all %d %d-letter words!\n", strand->numWords, strand->wordLen);   
            printf("Great job! Thanks for playing!\n");
            break;
        }
        // tell user how many words they have guessed correctly
        printf("You have found %d of %d %d-letter words in the strand so far.\n", strand->correct_num, strand->numWords, strand->wordLen);
    }
}
