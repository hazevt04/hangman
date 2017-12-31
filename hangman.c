#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>


#include "my_util.h"

#include "hangman.h"

// Project 1- COMP 3220
// Glenn Hazelwood
// 2017/10/28

// Implementations for functions for checking the secret word and
// for updating and showing the state of Hangman games

// Count number of non_alphas in secret word
void get_num_non_alphas( int *num_non_alphas, hangman_char_t *hm_chars, int num_chars ) {
   assert( hm_chars != NULL );

   int nnon_alphas = 0;
   for( int i = 0; i < num_chars; i++ ) {
      if ( hm_chars[i].is_non_alpha ) {
         nnon_alphas++;
      }
   }
   *num_non_alphas = nnon_alphas;
}




// Sets the hangman char array chars based on the secret word and
// none of the characters having been guessed
void set_hangman_char_array( hangman_char_t *hm_chars, char* secret_word, 
      int num_chars ) {

   assert( hm_chars != NULL );
   assert( secret_word != NULL );

   for( int i = 0; i < num_chars; i++ ) {
      hm_chars[i].c = secret_word[i];
      if ( isalpha( ( int )( secret_word[i] ) ) ) {
         hm_chars[i].is_non_alpha = 0;
         hm_chars[i].guessed = 0;
      } else {
         hm_chars[i].is_non_alpha = 1;
         hm_chars[i].guessed = 1;
      }
   } // end of for loop
} // set_hangman_char_array


// Set the hangman char array chars to NULL characters and
// none of the characters having been guessed
void clear_hangman_char_array( hangman_char_t *hm_chars, int num_chars ) {
   
   assert( hm_chars != NULL );

   for( int i = 0; i < num_chars; i++ ) {
      hm_chars[i].c = 0;
      if ( hm_chars[i].is_non_alpha ) {
         hm_chars[i].guessed = 1;
      } else {
         hm_chars[i].guessed = 0;
      }
   } 
}


// Updates the hm_chars array based on the indices in the indicies array to
// indicate that those characters of the secret word have been guessed.
// The function expects the caller to allocate hm_chars and indices
void update_hangman_char_array( hangman_char_t *hm_chars, int* indices, 
      int num_updates ) {
   
   assert( hm_chars != NULL );
   assert( indices != NULL );

   for(int i = 0; i < num_updates; i++ ) {
      int update_index = indices[i];
      hm_chars[update_index].guessed = 1;
   } 
}


// Check to see if guess_char is in the secret word in hm_chars
// and has not been already matched..
// Sets an array of indices for matches for the guess character.
// Match is case-insensitive.
// The function expects the caller to allocate hm_chars and match_indices
int check_guess( hangman_char_t *hm_chars, int* match_indices, int num_chars, 
      char guess_char ) {
   
   assert( hm_chars != NULL );

   int num_matches = 0;
   
   HDEBUG_PRINTF( "Inside %s(): Guess_char %c\n", __func__, guess_char ); 

   for( int i = 0; i < num_chars; i++ ) {
      int hm_lowerc = tolower(( int )hm_chars[i].c );
      int hm_upperc = toupper(( int )hm_chars[i].c );
      // Match is case insenstive
      if ( ( ( int )guess_char == hm_lowerc ) || ( ( int )guess_char == hm_upperc ) ) {
         // Only count as match if the character has not been guessed already
         if ( !hm_chars[i].guessed ) {
            match_indices[num_matches] = i;
            num_matches++;
         }
      }
   }

   HDEBUG_PRINTF( "Inside %s(): %d New Matches Found for %c\n", 
    __func__, num_matches, guess_char ); 
   return num_matches; 

} // end of check_guess


// Print previous guesses on one line, comma separated and in single quotes
void print_prev_guesses( char **prev_guesses, int num_guesses ) {
   assert( prev_guesses != NULL );

   for( int i = 0; i < num_guesses; i++ ) {
      assert( prev_guesses[i] != NULL );
      printf( "'%s'%c", prev_guesses[i], 
         ( i == ( num_guesses - 1 ) ) ? '.' : ',');
   }
}


// Prints the state of your guessing the secret word. It displays 
// blanks for characters you have not yet guessed.
void print_hangman_char_array( hangman_char_t *hm_chars, int num_hm_chars ) {
   for( int i = 0; i < num_hm_chars; i++ ) {
      printf( "%c ", hm_chars[i].guessed ? hm_chars[i].c : '_' ); 
   }
}

void print_secret_word( hangman_char_t *hm_chars, int num_hm_chars ) {
   for( int i = 0; i < num_hm_chars; i++ ) {
      printf( "%c", hm_chars[i].c ); 
   }
   
}

// "Flourish" As you guess incorrectly more body parts are
// added to the hangman figure.
void print_hangman_figure_line( int score, int figure_line ) {
   switch ( figure_line ) {
      case 0 :
         if ( score > 0 ) {
            printf( " o   " ); 
         } else {
            printf( "     " ); 
         }
         break;
      case 1 :
         if ( score > 3 ) {
            printf( "/|\\  " );
         } else if ( score > 2 ) {
            printf( "/|   " ); 
         } else if ( score > 1 ) {
            printf( " |   " ); 
         } else {
            printf( "     " ); 
         }
         break;
      case 2 :
         if ( score > 5 ) {
            printf( "/ \\  " );
         } else if ( score > 4 ) {
            printf( "/    " ); 
         } else { 
            printf( "     " ); 
         }
         break;
      default:
         fprintf( stderr, "ERROR: Invalid figure_line: %d\n",
            figure_line );
   } // end of switch( figure_line ) 

} // end of print_hangman_figure_line()


// Show the overall state of the Hangman game:
// Show the hangman figure,
// Which characters you've guessed and not guessed,
// bumber of guesses,
// and
// all of your previous guesses
void print_hangman_state( char *category_name, int secret_word_len, hangman_char_t* hm_chars, 
      int score, char **prev_guesses ) {
  
   assert( category_name != NULL );
   assert( hm_chars != NULL );
   assert( prev_guesses != NULL );
   assert( *prev_guesses != NULL );

   // Output Line 0
   print_hangman_figure_line( score, 0 );
   printf( "Category: %s  %d letters.", category_name, secret_word_len );
   printf( "\n" ); 
      
   // Output Line 1
   print_hangman_figure_line( score, 1 );
   print_hangman_char_array( hm_chars, secret_word_len );
   printf( "\n" ); 

   // Output Line 2
   print_hangman_figure_line( score, 2 );
   printf( "Number of Guesses: %d/%d", score, MAX_NUM_GUESSES );
   printf( "\n" );
   
   // Output Line 3
   printf( "     " );
   printf( "Previous Wrong Guesses: " );
   print_prev_guesses( prev_guesses, score  );         
   printf( "\n\n");         
}


// Check whether the candidate secret line
// has non-alphabetic/non-non_alphas characters or not
// It it does have non-alphabetic/non-non_alpha characters
// return 1, otherwise return 0
int is_secret_line_valid( char *secret_line ) {
   
   assert( secret_line != NULL );

   HDEBUG_PRINTF( "Inside %s(): Secret line being checked for validity is '%s'\n", 
         __func__, 
      secret_line );

   char *l_ptr = secret_line;

   HDEBUG_PRINTF( "Secret_line characters: " ); 
   while (  *l_ptr != '\0' ) {
      HDEBUG_PRINTF("%c ", *l_ptr ); 
      if ( ( !isprint( ( int )( *l_ptr ) ) ) ) {
         HDEBUG_PRINTF( "Inside %s(): \n\t\tsecret_line character %x "
            "is not a printable character.\n", __func__, ( int )( *l_ptr ) ); 
         return 0;
      }
      l_ptr++;
   }
   return 1;

} // end of is_secret_line_valid()


// Uses gsl_rand to select word file
void get_category_name( char* category_name, unsigned int seed ) {
   
   assert( category_name != NULL );

   int rand_index;
   
   gsl_rng *rng = gsl_rng_alloc( gsl_rng_taus );

   gsl_rng_set( rng, time( NULL ) );

   rand_index = gsl_rng_uniform_int( rng, NUM_CATEGORY_FILES );
   
#ifdef TEST
   rand_index = 0;
#endif   

   HDEBUG_PRINTF( "Index is %d\n", rand_index );  

   strcpy( category_name, categories[rand_index] ); 
   HDEBUG_PRINTF( "Selected random category is %s.\n", category_name ); 
   printf( "Selected random category is %s.\n", category_name ); 

}


// Uses gsl_rand() after it is seeded to select a line number in file_name
// to ve the secret word. Sets a pointer to a character array of that word
void get_secret_word( char* secret_word, char* category_name, unsigned int seed ) {

   assert( secret_word != NULL );
   assert( category_name != NULL );

   int secret_line_num = -1;
   int num_file_lines = 0;
   char secret_line[MAX_NUM_CHARS];
   
   char secret_word_path[MAX_NUM_CHARS];
   FILE* fp;

   sprintf( secret_word_path, "%s/.inputfiles/%s.txt", getenv( "PWD" ), category_name );

   HDEBUG_PRINTF( "Inside %s(): secret word path is %s\n", __func__, secret_word_path );   

   fp = fopen( secret_word_path, "r" );
   if ( !fp ) {
      HDEBUG_PRINTF( "All Characters in the file name: \n" ); 
      for( int i=0; i < MAX_NUM_CHARS; i++ ) {
         HDEBUG_PRINTF( "Inside %s(): \t%d: %c(%d)\n", __func__, i, secret_word_path[i], 
               secret_word_path[i] ); 
      } 
      HDEBUG_PRINTF( "\n" ); 
      HDEBUG_PRINTF( "Inside %s(): strlen(secret_word_path) is %zu\n", __func__, 
            strlen( secret_word_path ) ); 

      if ( errno == ENOENT ) {
         fprintf( stderr, "ERROR: The file %s was not found.\n", secret_word_path );
      } else {
         fprintf( stderr, "ERROR: The file %s could not be opened.\n", secret_word_path );
      }
      fclose( fp );
      exit( EXIT_FAILURE );
   }

   HDEBUG_PRINTF( "Inside %s(): secret_word_path is %s\n", __func__, secret_word_path );
   HDEBUG_PRINTF( "All Characters in the file name: \n" ); 
   for( int i = 0; i < MAX_NUM_CHARS; i++ ) {
      HDEBUG_PRINTF( "Inside %s(): \t%d: %c(%d)\n", __func__, i, secret_word_path[i], 
            secret_word_path[i] ); 
   } 
   HDEBUG_PRINTF( "\n" ); 
   HDEBUG_PRINTF( "Inside %s(): strlen(secret_word_path) is %zu\n", __func__, 
         strlen( secret_word_path ) );    
   
   get_num_file_lines( &num_file_lines, fp );
  
   int secret_line_valid = 0;
   int num_bad_lines = 0;
   gsl_rng *rng = gsl_rng_alloc( gsl_rng_taus2 );
   gsl_rng_set( rng, time( NULL ) );

   while ( !secret_line_valid ) {
   
      secret_line_num = gsl_rng_uniform_int( rng, num_file_lines );

#ifdef TEST
      secret_line_num = 0;
#endif


      HDEBUG_PRINTF( "seed is %u\n", seed );
      HDEBUG_PRINTF( "secret_line_num is %d\n", secret_line_num );
      
      HDEBUG_PRINTF( "Inside %s(): secret_line_num is %d\n", 
            __func__, secret_line_num ); 
  
      HDEBUG_PRINTF( "Inside %s(): Reading line num %d from %s\n", 
            __func__, secret_line_num, secret_word_path );  
   
      get_file_line( secret_line, secret_line_num, fp );
      secret_line_valid = is_secret_line_valid( secret_line );

      HDEBUG_PRINTF( "Inside %s(): Secret line is %s\n", __func__,
         ( secret_line_valid ) ? "valid" : "not valid" ); 

      if ( ( num_file_lines == 1 ) && ( !secret_line_valid ) ) {

         fprintf( stderr, "ERROR: The secret line file %s has 1 line "
            "and it is invalid:\n%s\n\n",
            secret_word_path, secret_line );
         printf( "Exiting.\n\n" );
         exit( EXIT_FAILURE ); 
      }

      if ( ( num_bad_lines == num_file_lines ) && ( !secret_line_valid ) ) {
         fprintf( stderr, "ERROR: The secret line file %s has %d lines "
            "and we've tried %d times to find a good line.\n\n",
            secret_word_path, num_file_lines, num_bad_lines);
         printf( "Exiting.\n\n" );
         exit( EXIT_FAILURE ); 
      }
      
      num_bad_lines++;

   } // end of while ( !secret_line_valid )

   // Basically renaming the variable 'secret_line' to 'secret_word'
   sprintf( secret_word, "%s", secret_line );
   
   HDEBUG_PRINTF( "Inside %s(): Shhh... The secret word is %s\n", 
         __func__, secret_word ); 

   gsl_rng_free( rng );
} // end of get_secret_word


// Save the guess line to later display all of the previous guesses
void save_guess( char** prev_guesses, char* guess_line, int guess_line_len, int num_guesses ) {

   assert( prev_guesses != NULL );

   for( int i = 0; i < guess_line_len; i++ ) {
      assert( prev_guesses[num_guesses] != NULL );
      
      HDEBUG_PRINTF( "Inside %s():\tguess_line[%d] = %c\n",
      
            __func__, i, guess_line[i] );
      
      prev_guesses[num_guesses][i] = guess_line[i];
      
      HDEBUG_PRINTF( "Inside %s():\tprev_guesses[%d][%d] = %c\n",
         __func__, num_guesses, i, prev_guesses[num_guesses][i] );
   } 
   prev_guesses[num_guesses][guess_line_len] = '\0';  
} // save_guess()
