# Makefile for Project 1 COMP3220
# Glenn Hazelwood

TARGET_EXEC = hangman
CC = cc
#DEBUG =-DHDEBUG 
CFLAGS = -g -I. $(DEBUG)

.PHONY: default all clean

default: $(TARGET_EXEC)
all: default	


# Reminder:
# $@ file name of the target of the rule
# $% target member name
# $< name of first prerequisite

# patsubst and wildcard allow this makefile to 
# automagically include all .c and *.h files 
# in current directory
OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# If make is killed or interrupted the targets,
# $(TARGET) and $(OBJECTS) are not deleted.
# Also makes sure to keep .o files around
.PRECIOUS: $(TARGET_EXEC) $(OBJECTS)

$(TARGET_EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $@ -lgsl -lgslcblas
<<<<<<< HEAD
=======

>>>>>>> categories

clean: clean_player_data
	-rm -f *.o
	-rm -f $(TARGET_EXEC)

clean_player_data:
	-rm -f ~/.hangman

# Just in case you don't have GSL RNG
get_gsl:
	sudo apt-get install libgsl-dev
	sudo apt-get install libgsl-dbg


check:
	echo "OBJECTS are $(OBJECTS)"
	echo "HEADERS are $(HEADERS)"

# Just in case you don't have GSL RNG
get_gsl:
	sudo apt-get install libgsl-dev
	sudo apt-get install libgsl-dbg

