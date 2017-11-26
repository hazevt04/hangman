#ifndef HANGMAN_H
#define HANGMAN_H

#define NUM_FILES 12

#define MAX_NUM_GUESSES 6 

// Number of Lines for the 
// Hangman figure
#define NUM_FIGURE_LINES 3

static char categories[][MAX_NUM_CHARS] = {
	"candy", 
	"cars",
   "colleges",
	"classroom",
	"fruits",
	"kitchen",
	"nbateams", 
	"nflteams", 
   "tools",
	"scifimovies", 
   "comedymovies",
   "soda"
   };


// Project 1- COMP 3220
// Glenn Hazelwood
// 2017/10/28

// Header for functions for checking the secret word and
// for updating and showing the state of Hangman games


// Struct for updating and displaying
// the state of the Hangman game
typedef struct {
   int guessed;
   int is_space;
   char c;
} hangman_char_t;


// Count the number of spaces to factor into check of number of characters'
// guessed
void get_num_spaces( int *num_spaces, hangman_char_t *hm_chars, int num_chars );


// Sets the hangman char array chars based on the secret word and
// none of the characters having been guessed
void set_hangman_char_array( hangman_char_t *hm_chars, char* secret_word, 
      int num_chars );


// Set the hangman char array chars to NULL characters and
// none of the characters having been guessed
void clear_hangman_char_array( hangman_char_t *hm_chars, int num_chars );


// Updates the hm_chars array based on the indices in the indicies array to
// indicate that those characters of the secret word have been guessed.
// The function expects the caller to allocate hm_chars and indices
void update_hangman_char_array( hangman_char_t *hm_chars, int* indices, 
      int num_updates );


// Check to see if guess_char is in the secret word in hm_chars
// and has not been already matched..
// Sets an array of indices for matches for the guess character.
// Match is case-insensitive.
// The function expects the caller to allocate hm_chars and match_indices
int check_guess( hangman_char_t *hm_chars, int* match_indices, int num_chars, 
      char guess_char );


// Print one line with all of your previous guesses, each one in single quotes
void print_prev_guesses( char **prev_guesses, int num_prev_guesses ); 


// Prints the state of your guessing the secret word. It displays 
// blanks for characters you have not yet guessed.
void print_hangman_char_array( hangman_char_t *hm_chars, int num_hm_chars );


// Prints the revealed secret word for the loser
void print_secret_word( hangman_char_t *hm_chars, int num_hm_chars );


// "Flourish" As you guess incorrectly more body parts are
// added to the hangman figure.
void print_hangman_figure_line( int score, int figure_line );


// Show the hangman figure,
// Which characters you've guessed and not guessed,
// bumber of guesses,
// and
// all of your previous guesses in one line
void print_hangman_state( char *category_name, int secret_word_len, hangman_char_t* hm_chars, 
      int score, char **prev_guesses );


// Check whether the candidate secret line
// has non-alphabetic characters or not
// It it does have non-alphabetic characters
// return 1, otherwise return 0
int is_secret_line_valid( char *secret_line );


// Pick a category (all lowercase). There should then be 
// a <category_name>.txt file in the .input_files directory
void get_category_name( char* file_name, unsigned int seed );


// Uses rand() after it is seeded to select a line number in file_name
// to ve the secret word. Sets a pointer to a character array of that word
void get_secret_word( char* secret_word, char* category_name, unsigned int seed );


// Save the guess line to later display all of the previous guesses
void save_guess( char** prev_guesses, char* guess_line, int guess_line_len, int num_guesses );


#endif // HANGMAN_H
