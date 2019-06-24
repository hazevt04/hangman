#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <errno.h>

#include "my_util.h"

// Project 1- COMP 3220
// Glenn Hazelwood
// 2017/10/28

// Implementations for functions I commonly reuse for
// programming assignments for COMP 3220


// Sets the number of lines in the file pointed to by fp
void get_num_file_lines( int* num_lines, FILE *fp ) {

   int l_num_lines = 0;
   char line[MAX_NUM_CHARS];

   while( fgets( line, MAX_NUM_CHARS, fp ) != NULL ) {
      l_num_lines++;
   } 

   // Reset file pointer back to the beginning of the file
   fseek( fp, 0, 0 );

   // Set file size
   // Not expecting to actually need
   // 64-bits for file size; ok with 32 bits.
   *num_lines = ( int )l_num_lines;

} // end of get_num_file_lines


// Sets a pointer to a character array for the line_num+1-th line 
// of the file pointed to by fp
int get_file_line( char *file_line, int line_num, FILE *fp ) {
   
   char c;
   int line_count = 0;
   char line[MAX_NUM_CHARS];

   // Move file pointer to the line you want
   while ( fgets( line, MAX_NUM_CHARS, fp ) != NULL ) {
      if ( line_count != line_num ) {
         line_count++;
      } else {
         break;
      }
   } // end of while

   // Replace newline in line read from file with null
   line[strlen( line ) - 1] = '\0';

   HDEBUG_PRINTF( "Inside %s(): We wanted line %d.\n", __func__, line_num ); 
   HDEBUG_PRINTF( "Inside %s(): We got line %d: %s\n", __func__, line_count, line ); 

   sprintf( file_line, "%s", line );

   HDEBUG_PRINTF( "Inside %s(): file line set to %s\n", __func__, file_line ); 

} // end of get_file_line
