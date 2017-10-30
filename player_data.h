#ifndef PLAYER_DATA_H
#define PLAYER_DATA_H

// Project 1- COMP 3220
// Glenn Hazelwood
// 2017/10/28

// Header file for types and functions
// related to keeping track of the players
// stats for Hangman games

// Struct for saving
// data on the player
typedef struct {
   int num_games;
   int num_losses;
   int num_wins;
   int total_win_score;
   double total_time;
   double avg_win_score;
   double avg_time_per_game;
} player_data_t;


// Returns ENOENT if player data file not found
int check_for_player_data_file( char *file_name );

// Initial data written if there is no player data 
// file
void init_player_data( player_data_t *player_data );

// Writes data from player_data struct to a binary file
void write_player_data_to_file( char* file_name, player_data_t *player_data );


// Reads player data from binary file and puts in a player_data struct
void read_player_data_from_file( char* file_name, player_data_t *player_data );

// Set the player_data (based om the result of the game)
// This does not modify the player data file.
void update_player_data( int loss, int win, int win_score, double time, player_data_t *player_data );

// Displays the player stats on one line
// Game #. # Win(s)/# Loss(es). Average Win Score: #.##. Average Time Per Game: #.# Seconds.
void print_player_data( player_data_t *player_data );

#endif // PLAYER_DATA_H
