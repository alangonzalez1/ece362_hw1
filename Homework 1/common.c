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
#include <stdlib.h>
#include <time.h>

// Constants

// prototypes for the Helper functions
static int random_between_min_and_max(int min, int max);

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

	// give error if position exceeds the amount of characters	
	long int max = strlen(str) - 1; // should be unable to print the \0 character
	if(pos > max)
	{
		fprintf(stderr, "get_char_from_str: desired position exceeds length of str\n");
		exit(1);
	}
	int i;
	for(i=0; i<pos; i++)
	{
		*++str;
	}

	return (char) *str;
}

/**
 * err() - generates desired error message and exits the program
 * 
 * @return nothing
 * 
 * @param errmsg - pointer to a string that states the desired error message
 * 
 */
 
void err(char* errmsg)
{
	fprintf(stderr, errmsg);
	exit(1);
}

/**
 * randn() - generates random number between min and max
 * 
 * @return randomly generated number between min and max
 * 
 * @param min - minimum number to be randomly generated
 * @param max - maximum number to be randomly generated
 * 
 */
 
int randn(int min, int max)
{
	if (max < min)
	{ 
		err("ERROR randn: minimum exceeds maximum\n");
	}
	int n = random_between_min_and_max(min, max);
	return n;
}

/**
 * printIntArr() - prints elements in an integer array
 * 
 * @return nothing
 * 
 * @param array - pointer to an integer array
 * @param n - number of elements in the array
 * 
 */
void printIntArr(int* array, int n)
{
	int i;
	for(i=0; i<n; ++i)
	{
		printf("\t%d\n", array[i]);
	}
}

/**
 * pos_min() - gives the position of the minimum number in the list 
 * 
 * @return position integer of the minimum number in the list
 * 
 * @param list - pointer to the list that will be traversed
 * @param n - number of elements in the list
 * 
 */
int pos_min(int list[], int n) 
{
	int i; // needed in for loop
	int pos = 0; // position of the minimum number in the list
	int min = list[pos]; // set the min to be the first entry in the array
	
	for (i=0; i<n; ++i)
	{
		if(min > list[i])
		{
			min = list[i]; // update min if new min found
			pos = i; // update position if new min found
		}
	}
	return pos;
}

/**
 * pos_max() - gives the position of the maximum number in the list
 * 
 * @return position integer of the maximum number in the list
 * 
 * @param list - pointer to the list that will be traversed
 * @param n - number of elements in the list
 * 
 */
int pos_max(int list[], int n)
{
	int i; // needed in for loop
	int pos = 0; // position of the maximum number in the list
	int max = list[0]; // set the max to be first entry in array
	
	for(i=0; i<n; ++i)
	{
		if(max < list[i])
		{
			max = list[i]; // update max if new max found
			pos = i; // update position if new max found
		}
	}
	return pos;
}


/**
 * pos_max() - gives the position of the maximum in the number 
 * 
 * @return position integer of the maximum number in the list
 * 
 * @param list - pointer to the list that will be traversed
 * @param n - number of elements in the list
 * 
 */
int maximum(int list[], int n) 
{
	int i; // needed in for loop
	int max = list[0]; // set the max to be the first array in the element
	
	for (i=0; i<n; ++i)
	{
		if(max < list[i])
			max = list[i]; // update max if new max found
	}
	return max;
}


// Helper function prototypes

static int random_between_min_and_max(int min, int max)
{
	return (rand() % (max - min + 1)) + min;
}
	
