#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <errno.h>

#include "my_util.h"
#include "player_data.h"
#include "hangman.h"

// Project 1 COMP 3220
// Glenn Hazelwood
// 2017/10/27

// Hangman Game
// Usage: hangman <optional filename for secret words>
// Default secret word file is ~/.words
// Saves game stats in ~/.hangman
// to allow tracking accross games

int main( int argc, char** argv ) {
   
   int *match_indices;
   int num_matches = 0;
   int num_total_matches = 0;

   char secret_word[MAX_NUM_CHARS];
   int secret_word_len;
   
   char guess_char;
   char guess_line[MAX_NUM_CHARS];

   char file_name[MAX_NUM_CHARS];
   char default_file_name[MAX_NUM_CHARS];
   char file_line[MAX_NUM_CHARS];

   char **prev_guesses;
   unsigned int seed = ( unsigned int )time(0);
   
   prev_guesses = ( char** )malloc( MAX_NUM_GUESSES * sizeof( char* ) );
   for ( int i = 0; i < MAX_NUM_GUESSES; i++ ) {
      prev_guesses[i] = ( char* )malloc( MAX_NUM_CHARS * sizeof( char ) );    
   }	

   hangman_char_t *hm_chars;
   player_data_t player_data;

   /////////////////////////
   // GET THE SECRET WORD
   /////////////////////////
   // Default path to the file with the secret words  
   sprintf( default_file_name, "/home/%s/.words", getenv("USER") );
   
   HDEBUG_PRINTF( "Inside %s(): Default_file_name is %s\n", __func__, default_file_name ); 
   HDEBUG_PRINTF( "Inside %s(): strlen for default_file_name is %zu\n", __func__, strlen( default_file_name ) ); 

   if ( argc < 2 ) {
      HDEBUG_PRINTF( "Inside %s(): Assuming default file_name: %s\n", __func__, default_file_name );
      strcpy( file_name, default_file_name ); 
   } else { 
      strcpy( file_name, argv[1] );
   }
   HDEBUG_PRINTF( "Inside %s(): file_name is %s\n", __func__, file_name ); 
   HDEBUG_PRINTF( "Inside %s(): strlen for file_name is %zu\n", __func__, strlen( file_name ) ); 

   // Use the CPU time to seed the RNG for selecting the secret word
   get_secret_word( secret_word, file_name, seed );
     
   secret_word_len = strlen( secret_word );
   
   MALLOC_AND_CHECK_ERROR( match_indices, int, ( secret_word_len * sizeof( int ) ) );

   MALLOC_AND_CHECK_ERROR( hm_chars, hangman_char_t, ( secret_word_len * sizeof( hangman_char_t ) ) );

   set_hangman_char_array( hm_chars, secret_word, secret_word_len );

   num_total_matches = 0;

   ////////////////////////////////////
   // GET THE STATS FOR THE PLAYER
   ////////////////////////////////////
   char player_data_file_name[MAX_NUM_CHARS];
   // Path to the file saving the player data across games
   sprintf( player_data_file_name, "/home/%s/.hangman", getenv( "USER" ) ); 

   // If there is not already a player data file. Make one with the data
   // initialized so it can be read and shown to you.
   if ( check_for_player_data_file( player_data_file_name ) == ENOENT ) {
      init_player_data( &player_data );
      write_player_data_to_file( player_data_file_name, &player_data );

      // No need to re-read the file, the data is already in player_data
      // struct
   } else {
      read_player_data_from_file( player_data_file_name, &player_data );
   }

   // Shows Stats on the player like Game Number, Wins/Losses, Avg Score for wins
   // Avg time (in seconds) for wins.
   print_player_data( &player_data ); 
  
   double duration = 0.0;
   int score = 0;

   /////////////////////
   // PLAY THE GAME
   ////////////////////
   while( 1 ) {
      
      clock_t start_time;
      clock_t end_time;
		int guess_line_len;
      
      start_time = clock();

      HDEBUG_PRINTF( "Inside %s(): The secret word has %d characters.\n\n\n", __func__, secret_word_len ); 
      HDEBUG_PRINTF( "Inside %s(): You have %d guesses left\n", __func__, ( MAX_NUM_GUESSES - score ) ); 
     
      system("cls") && system("clear");

      // Print player stats
      print_player_data( &player_data );
      printf("\n"); 

      // Show the overall state of the game
      print_hangman_state( secret_word_len, hm_chars, score, prev_guesses );


      /////////////////////
      // GET THE GUESS
      ////////////////////
      // Prompt user for a guess
      printf( "     Your guess: " );        
      fgets( guess_line, MAX_NUM_CHARS, stdin );
      printf( "\n" );
      
      guess_line_len = strlen( guess_line );
      guess_line[guess_line_len - 1] = '\0';
      
      guess_char = guess_line[0];

      HDEBUG_PRINTF( "Inside %s(): Guess is %c\n", __func__, guess_char ); 
      
      // Valid or not, save the guess to show later
      save_guess( prev_guesses, guess_line, guess_line_len, score );
      
      HDEBUG_PRINTF( "Inside %s(): Score is %d. MAX_NUM_GUESSES is %d\n", __func__, 
         score, MAX_NUM_GUESSES );
      
      if ( !isalpha( ( unsigned char )guess_char ) ) {
         score++;
         
         HDEBUG_PRINTF( "Inside %s(): %c is invalid. Guess must be a letter\n", __func__, guess_char ); 
			HDEBUG_PRINTF( "Inside %s(): Score becomes %d\n", __func__, score ); 
         // Too many missed guesses; You LOSE
         if ( score == MAX_NUM_GUESSES ) {
            end_time = clock();
            duration = ( ( double )end_time - ( double )start_time );
            HDEBUG_PRINTF( "Inside %s(): Duration was %f\n", __func__, duration ); 
          
            system("cls") && system("clear");
            // Print player stats
            print_player_data( &player_data );
            printf("\n"); 
            
            // Show the overall state of the game
            print_hangman_state( secret_word_len, hm_chars, score, prev_guesses );
            
            printf( "     You LOSE!\n" ); 
            printf( "     The word was " );
            print_secret_word( hm_chars, secret_word_len );
            printf(".\n");	
            
            // In the case of a loss, don't add to score
            //               loss,win,score
            update_player_data( 1, 0, 0, duration, &player_data );
            write_player_data_to_file( player_data_file_name, &player_data );
            break;
         
         } else {
            continue;
         }
      } // end of if ( !isalpha( ( unsigned char )guess_char ) )

      //////////////////////////
      // CHECK THE GUESS
      //////////////////////////
      HDEBUG_PRINTF( "Inside %s(): num_total_matches = %d before checking the guess.\n", 
         __func__, num_total_matches ); 

      num_matches = check_guess( hm_chars, match_indices, secret_word_len, guess_char );

      if ( num_matches > 0 ) {
         HDEBUG_PRINTF( "Inside %s(): Found %d matches for %c in the secret word!\n", __func__,
            num_matches, guess_char ); 
         
         update_hangman_char_array( hm_chars, match_indices, num_matches ); 
         num_total_matches += num_matches;
         
         HDEBUG_PRINTF( "Inside %s(): num_total_matches = %d\n", __func__, num_total_matches ); 
         HDEBUG_PRINTF( "Inside %s(): secret_word_len is %d\n\n\n", __func__, secret_word_len ); 
        
         ///////////////////////// 
         // USER WON
         ///////////////////////// 
         if ( ( secret_word_len - num_total_matches ) == 0 ) {
            end_time = clock();
            duration = ( (double)end_time - (double)start_time );
            HDEBUG_PRINTF( "Inside %s(): end_time is %f\n", __func__, (double)end_time ); 
            HDEBUG_PRINTF( "Inside %s(): start_time is %f\n", __func__, (double)start_time ); 
            HDEBUG_PRINTF( "Inside %s(): Duration was %f\n", __func__, duration ); 

            system("cls") && system("clear");
            // Show the overall state of the game
            // Print player stats
            print_player_data( &player_data );
            printf("\n"); 

            print_hangman_state( secret_word_len, hm_chars, score, prev_guesses );
            
            printf( "     You WIN! You had %d %s.\n\n", score,
		         ( score == 1 ? "miss" : "misses" ) ); 

            // In the case of a win, add to score
            //               loss, win
            update_player_data( 0, 1, score, duration, &player_data );
            write_player_data_to_file( player_data_file_name, &player_data );
            num_total_matches = 0;
            break;
         }

      } else { // if no match			
         score++;
         
         HDEBUG_PRINTF( "Inside %s(): %c not in secret word.\n", __func__, guess_char );
         HDEBUG_PRINTF( "Inside %s(): You have %d guesses left.\n", __func__, ( MAX_NUM_GUESSES - score ) ); 
         HDEBUG_PRINTF( "Inside %s(): Score became %d\n\n", __func__, score ); 

         ///////////////////////// 
         // USER LOST
         ///////////////////////// 
         if ( score == MAX_NUM_GUESSES ) {
            end_time = clock();
            duration = ( ( double )end_time - ( double )start_time );
            HDEBUG_PRINTF( "Inside %s(): Duration was %f\n", __func__, duration ); 
          
            system("cls") && system("clear");
            // Print player stats
            print_player_data( &player_data );
            printf("\n"); 
            
            // Show the overall state of the game
            print_hangman_state( secret_word_len, hm_chars, score, prev_guesses );
            
            printf( "     You LOSE!\n" ); 
            printf( "     The word was " );
            print_secret_word( hm_chars, secret_word_len );
            printf(".\n\n");	
            
            // In the case of a loss, don't add to score
            //               loss,win,score
            update_player_data( 1, 0, 0, duration, &player_data );
            write_player_data_to_file( player_data_file_name, &player_data );
            break;
         } 
      }
   } // end of while loop


   ///////////////////
   // CLEANUP
   ///////////////////
   FREE_AND_NULL_PTR( match_indices );
   FREE_AND_NULL_PTR( hm_chars );

   for( int i = 0; i < MAX_NUM_GUESSES; i++ ) {
      FREE_AND_NULL_PTR( prev_guesses[i] );
   }
   FREE_AND_NULL_PTR( prev_guesses );

   return EXIT_SUCCESS;
}
