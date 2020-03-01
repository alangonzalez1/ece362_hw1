/*
 * common.h - Header file for commonly needed functions
 * 
 * Author: Alan Gonzalez
 * 
 * This is the header file for the most commonly used parts of a program
 * made into functions.
 *
 */

# ifndef _COMMON_H_
# define _COMMON_H_

// constants

// functions prototypes
char get_char_from_str(char* str, int pos);
void err(char* errmsg);
int randn(int min, int max);
void printIntArr(int* array, int n);
int pos_min(int list[], int n);
int pos_max(int list[], int n);
int maximum(int list[], int n);

#endif
