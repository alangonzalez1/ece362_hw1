/*
 * common.h - Source code file for commonly needed functions
 * 
 * Author: Alan Gonzalez
 * 
 * This is the source code file for the most commonly used parts of a program
 * made into functions.
 *
 */


// Include the header files
#include <stdio.h>
#include <string.h>

// Constants

// Prototypes for any needed helper functions

// Function definitions

/**
 * get_char_from_str() - returns a character from a string
 * 
 * @return pointer to a character at the desired position of a string
 * 
 * @param str - a pointer to a string
 * @param pos - the desired integer position of the character in a string
 * 
 */

char get_char_from_str(char* str, int pos)
{
	
	for(int i=0; i<pos; i++)
	{
		*++str;
	}

	return (char) *str;
}
