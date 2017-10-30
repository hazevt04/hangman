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

// Project 1- COMP 3220
// Glenn Hazelwood
// 2017/10/28

// Implementations of functions
// related to keeping track of the players
// stats for Hangman games

// Check to see if there is an existing player
// data file. Don't exit if the file isn't found.
int check_for_player_data_file( char *file_name ) {

   assert( file_name != NULL );

   FILE* fp;
   fp = fopen( file_name, "rb" );
   
   if( fp == NULL ) {
      strerror( errno );
   }
   return errno;
   
} // end of check_for_player_data_file


// Initial data to be written to the player data file 
// if there was no player data file already
void init_player_data( player_data_t *player_data ) {

   assert( player_data != NULL );

   player_data->num_games = 1;
   player_data->num_losses = 0;
   player_data->num_wins = 0;
   player_data->total_win_score = 0;
   player_data->total_time = 0.0;
   player_data->avg_win_score = 0.0;
   player_data->avg_time_per_game = 0.0;

} // end of init_player_data


// Write the player data struct to a binary file
void write_player_data_to_file( char* file_name, player_data_t *player_data ) {

   assert( file_name != NULL );
   assert( player_data != NULL );
  
   FILE* fp;
   fp = fopen( file_name, "wb" );

   HDEBUG_PRINTF( "Inside %s(): file_name is %s\n", __func__, file_name );
   HDEBUG_PRINTF( "Inside %s(): strlen(file_name) is %zu\n", __func__, strlen( file_name ) ); 
   if ( !fp ) {
      HDEBUG_PRINTF( "All Characters in the file name: \n" ); 
      for(int i = 0; i < MAX_NUM_CHARS; i++ ) {
         HDEBUG_PRINTF( "Inside %s(): \t%d: %c(%d)\n", __func__, i, file_name[i], file_name[i] ); 
      } 
      HDEBUG_PRINTF( "\n" ); 
      perror( "ERROR: " );
      exit( EXIT_FAILURE );
   }

   HDEBUG_PRINTF( "All Characters in the file name: \n" ); 
   for( int i = 0; i < MAX_NUM_CHARS; i++ ) {
      HDEBUG_PRINTF( "Inside %s(): \t%d: %c(%d)\n", __func__, i, file_name[i], file_name[i] ); 
   } 
   HDEBUG_PRINTF( "\n" ); 

   fwrite( player_data, sizeof( player_data_t ), 1, fp );
   fclose( fp );

} // end of write_player_data_to_file


// Write the player data struct to a binary file
void read_player_data_from_file( char* file_name, player_data_t *player_data ) {
   
   assert( file_name != NULL );
   
   char file_line[MAX_NUM_CHARS];

   FILE* fp;

   fp = fopen( file_name,"rb" );   
   if ( !fp ) {
      HDEBUG_PRINTF( "ALL CHARS IN FILE_NAME!: \n" ); 
      for(int i = 0; i < MAX_NUM_CHARS; i++ ) {
         HDEBUG_PRINTF( "Inside %s(): \t%d: %c(%d)\n", __func__, i, file_name[i], file_name[i] ); 
      } 
      HDEBUG_PRINTF( "\n" ); 
      HDEBUG_PRINTF( "Inside %s(): strlen(file_name) is %u\n", __func__, strlen( file_name ) ); 

      perror( "ERROR: " );
      exit( EXIT_FAILURE );
   }

   HDEBUG_PRINTF( "Inside %s(): file_name is %s\n", __func__, file_name );
   HDEBUG_PRINTF( "All Characters in the file name: \n" ); 
   for( int i = 0; i < MAX_NUM_CHARS; i++ ) {
      HDEBUG_PRINTF( "Inside %s(): \t%d: %c(%d)\n", __func__, i, file_name[i], file_name[i] ); 
   } 
   HDEBUG_PRINTF( "\n" ); 
   HDEBUG_PRINTF( "Inside %s(): strlen(file_name) is %u\n", __func__, strlen( file_name ) );    
   
   while( ( fread( player_data, sizeof( player_data_t ), 1, fp ) > 0 ) ) {} 
   fclose( fp );

} // end of read_player_data_from_file


// Set the player_data (based om the result of the game)
// This does not modify the player data file.
void update_player_data( int loss, int win, int win_score, double time, player_data_t *player_data ) {
   
   assert( player_data != NULL );

   player_data->num_losses += loss;
   player_data->num_wins += win;
   player_data->total_win_score += win_score;
   player_data->total_time += time;
   player_data->avg_win_score = ( double )player_data->total_win_score / ( double )player_data->num_games;
   player_data->avg_time_per_game = ( double )player_data->total_time / ( double )player_data->num_games;
     
   HDEBUG_PRINTF( "Inside %s(): player_data->num_losses = %d\n", __func__, player_data->num_losses );  
   HDEBUG_PRINTF( "Inside %s(): player_data->num_wins = %d\n", __func__, player_data->num_wins );  
   HDEBUG_PRINTF( "Inside %s(): player_data->total_win_score = %d\n", __func__, player_data->total_win_score );  
   HDEBUG_PRINTF( "Inside %s(): player_data->total_time = %f\n", __func__, player_data->total_time );  
   HDEBUG_PRINTF( "Inside %s(): player_data->avg_win_score = %f\n", __func__, player_data->avg_win_score );  
   HDEBUG_PRINTF( "Inside %s(): player_data->avg_time_per_game = %f\n", __func__, player_data->avg_time_per_game );  
   
   player_data->num_games++;
}


// Displays the player stats on one line
// Game #. # Win(s)/# Loss(es). Average Win Score: #.##. Average Time Per Game: #.# Seconds. 
void print_player_data( player_data_t *player_data ) {

   char win_str[MAX_NUM_CHARS];
   char loss_str[MAX_NUM_CHARS];
   char time_unit_str[MAX_NUM_CHARS];
   
   strcpy( win_str, ( ( player_data->num_wins == 1 ) ? "Win" : "Wins" ) );
   strcpy( loss_str, ( ( player_data->num_losses == 1 ) ? "Loss" : "Losses" ) );
   strcpy( time_unit_str, ( ( player_data->total_time == 1.0 ) ? "Second" : "Seconds" ) );

   //player_data->avg_win_score = (double)player_data->total_win_score/(double)player_data->num_games;
   //player_data->avg_time_per_game = (double)player_data->total_time/(double)player_data->num_games;
    
   printf( "Game: %d. ", player_data->num_games );
   printf( "%d %s/", player_data->num_wins, win_str );
   printf( "%d %s.   ", player_data->num_losses, loss_str );
   printf( "Average Win Score: %4.3f.    ", player_data->avg_win_score );
   printf( "Average Time Per Game: %8.4f %s.\n", 
      player_data->avg_time_per_game, time_unit_str );
}

