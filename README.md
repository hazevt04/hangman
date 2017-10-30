# Hangman
-------------------

by Glenn Hazelwood
2017/10/28
Originally for Project 1- COMP 3220 Intermediate C

Program to play hangman.
User has 6 guesses to guess secret word.
As user guesses wrong a limb is added to
a displayed stick figure.
 
A guess that you already made is
also considered wrong, by the way.

# Usage
-------------------

Usage: hangman <filename>

Default for filename is ~/.words

File must have one word per line and only
alphabets (uppercase or lowercase) in each word.


# Example Input Files Descriptions :
-------------------

one\_word.txt - One valid word. Good for a simple test of basic functionality

more\_words.txt - Words of various lengths

hard\_words.txt - Longer or more obscure words. (When in doubt guess 'z', 'q', v' or 'y')

one\_invalid\_line.txt- test case of a file with one line that has non-alphabetic characters. 
   hangman should exit upon reading this file

one\_line\_multi\_word.txt- test case of a file with one line that has multiple words on one line.
   hangman should exit upon reading this file

single\_char\_words.txt- test case of single character words.

