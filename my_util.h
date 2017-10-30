#ifndef MY_UTIL_H
#define MY_UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

// Glenn Hazelwood

// Utility functions and macros I like to use
// in my C code

#define NUM_INT_BITS (8*sizeof(int))
#define MAX_NUM_CHARS 80   

#ifndef MALLOC_AND_CHECK_ERROR 
#   define MALLOC_AND_CHECK_ERROR(ptr, type, nbytes) \
      ptr = (type *)malloc(nbytes); \
      if (!ptr) { \
         fprintf(stderr, "In %s on line %d, function %s: " \
            "ERROR: malloc of " #ptr \
            " of type " #type \
            " for %zu bytes failed.\n", \
             __FILE__, __LINE__, __func__, nbytes ); \
         exit(EXIT_FAILURE); \
      }
#endif

#ifndef FREE_AND_NULL_PTR
#   define FREE_AND_NULL_PTR(ptr) \
      free(ptr); \
      ptr = NULL; 
#endif

#ifndef HDEBUG_PRINTF
#  ifdef HDEBUG
#     define HDEBUG_PRINTF(fmt, ...) {\
         printf(fmt, ##__VA_ARGS__); \
      }
#  else
#     define HDEBUG_PRINTF(fmt, ...) {\
         ; \
      }
#  endif
#endif


#ifndef FUNC_AND_CHECK_ERROR
#   define FUNC_AND_CHECK_ERROR(fun) {\
      if ((fun) != EXIT_SUCCESS) { \
         fprintf(stderr, "In %s on line %d, function %s: ERROR " #fun \
              " failed.\n", \
             __FILE__, __LINE__, __func__ ); \
         exit (EXIT_FAILURE); \
      } \
}
#endif

#ifndef CHECK_PTR
#  define CHECK_PTR(ptr) {\
      if (!ptr) { \
         fprintf(stderr, "In %s on line %d, function %s: ERROR " #ptr \
              " is NULL. Malloc failed?\n", \
             __FILE__, __LINE__, __func__ ); \
         exit (EXIT_FAILURE); \
      } \
}
#endif

// Sets the number of lines in the file pointed to by fp
void get_num_file_lines( int* num_lines, FILE *fp );

// Sets a pointer to a character array for the line_num+1-th line 
// of the file pointed to by fp
int get_file_line( char *file_line, int line_num, FILE *fp );


#endif // MY_UTIL_H
