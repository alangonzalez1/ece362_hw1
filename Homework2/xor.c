#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include "common.h"


// constants
#define DEFAULT 0
#define FILESRC 1

int main(int argc, char *argv[])
{
	// Initialize the flags
	int in_src = DEFAULT; 							// input source flag
	int out_src = DEFAULT; 							// output source flag
	char* mask = (char*) calloc(20, sizeof(char)); 	// allocate memory for mask string
	strcpy(mask, "\0"); 							// initialize mask with end character
	
	// Values needed in the flag-updating section of the program
	int argvpos = 0; 								// start at argv[0]
	int argcpos = 1; 
	char* i_filename = "0"; 						// storage place for the name of the input file
	char* o_filename = "0"; 						// storage place for the name of the output file
	
	int m_updated = 0; // used to check if mask was already updated
	
////////////////////////////////////////////////////////////////////////	
//////////// update flags based on user-inputted arguments /////////////
////////////////////////////////////////////////////////////////////////
	
	while(argcpos < argc)
	{
		int char_pos = 0; // position of the character in the string
		
		// go to the first character
		char* temp = argv[++argvpos]; // start at argv[1]
		
		// look for the "-" character
		if(get_char_from_str(temp, char_pos) == '-')
		{
						// check next character 
			switch(get_char_from_str(temp, char_pos+1))
			{
				case 'i': // the 'i' character was found
					
					// check if input source flag has already been updated
					if(in_src > 0)
					{
						++argcpos; // to keep the count on track
						break; // if so, don't update again
					}
					in_src = FILESRC; // update the input source flag
					
					// store the input filename string
					++argcpos;
					if(argcpos >= argc)
					{
						err("ERROR xor: no input for argument\n");
					}
					i_filename = argv[++argvpos];
					
					break;
					
				case 'o': // the 'o' character was found
					
					// check if output source flag has already been updated
					if(out_src > 0)
					{
						++argcpos; // to keep the string count on track
						break; // if so, don't update again
					}
					
					// store the output filename string
					out_src = FILESRC; // update the output source flag
					
					// store the output filename string
					++argcpos;
					if(argcpos >= argc)
					{
						err("ERROR xor: no input for argument\n");
					}
					o_filename = argv[++argvpos];
					
					break;
				
				default:
					// should also set the mask
					// check if the mask flag has already been updated
					if(m_updated > 0)
					{
						++argcpos; // to keep the string count on track
						break; // if so, don't update again
					}
			
					// a mask will be made from this input
					mask = argv[argvpos];
					++m_updated; // set the flag to not update the mask variable again
					break;
					
			} // end switch
		} // end if
		
		else
		{
			// check if the mask flag has already been updated
			if(m_updated > 0)
					{
						++argcpos; // to keep the string count on track
						break; // if so, don't update again
					}
			
			// a mask will be made from this input
			mask = argv[argvpos];
			++m_updated; // set the flag to not update the mask variable again
			
		} // end else
		++argcpos;
		
	} // end while
	
	// check to see if the mask was updated, error if not updated
	if(m_updated == 0 | strlen(mask) > 9)
	{
		err("ERROR xor: mask must be provided and be less than 10 characters\n");
	}
	
////////////////////////////////////////////////////////////////////////
////////////////// Load in data from desired source ////////////////////
////////////////////////////////////////////////////////////////////////
	
	char* buf = (char*) calloc(1024, sizeof(char)); // set aside 1Kb of char space
	
	// check the input source flag
	if(in_src == FILESRC)
	{
		// load in data from a specified file
		int fd = open(i_filename, O_RDONLY); // open the file
		
		// if unable to open the specified file
		if(fd < 0)
		{
			errno = ENOENT;
			perror(argv[0]);
			exit(1);
		}
		
		// at this point, safe to assume that we were able to open file
		int size = read(fd, buf, 1024); 			// read from filename, store in buf
		
		// cut the newline from the string if it has it
		if(buf[strlen(buf) - 1] == '\n')
			buf[strlen(buf) - 1] = '\0'; // trim the string to remove the newline character

		close(fd); 									// close the file
		
	}
	
	else
	{
		// load in data from stdin
		int size = read(STDIN_FILENO, buf, 1024); 	// read from STDIN
		
		// cut the newline from the string if it has it
		if(buf[strlen(buf) - 1] == '\n')
			buf[strlen(buf) - 1] = '\0'; // trim the string to remove the newline character
		
	}

	// error if size of the mask exceeds string length
	if(strlen(mask) > strlen(buf))
	{
		err("ERROR xor: mask size cannot exceed number of characters in string\n");
	}
	
////////////////////////////////////////////////////////////////////////
////////////////////////// Modify the input ////////////////////////////
////////////////////////////////////////////////////////////////////////
	
	// initialize the needed variables to modify the input
	char* result = (char*) calloc(1024, sizeof(char)); 	// to store the result
	int counter = 0; 									// to keep track of how many characters read
	char ch; 											// character that we will be working with
	char m_ch; 											// mask character that we will be working with
	char r_ch;											// character that results from XOR'ing the string
														//	and mask characters
	int maskSize = strlen(mask); 						// to loop through the mask
	int i; 												// used in the for loop
	
	// main algorithm to modify the input
	while(counter < strlen(buf))
	{
		for(i=0; i<maskSize; ++i)
		{
			// check if all characters from buffer have been XOR'ed
			if(counter > strlen(buf) - 1)
				break; // terminate early if you run out of characters
			
			// to loop through the characters in the mask
			ch = get_char_from_str(buf, counter); 		// get a character from the input string
			m_ch = get_char_from_str(mask, i);			// get a character from the mask string
			r_ch = ch ^ m_ch;							// XOR the characters
			strncat(result, &r_ch, 1); 					// store in result string
			++counter;									// update the counter after grabbing char from buffer
		} // end for
	} // end while
	
////////////////////////////////////////////////////////////////////////
////////////////// Output result to desired location ///////////////////
////////////////////////////////////////////////////////////////////////

	// check output source flag
	if(out_src == FILESRC)
	{
		// output data to specified file
		int fd = open(o_filename, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU); 
		// Open a file, write only, create it if needed
		// user will have read, write and execute permissions
		
		// if unable to open the specified file
		if(fd < 0)
		{
			errno = ENOENT;
			perror(argv[0]);
			exit(1);
		}
		
		
		
		// at this point, safe to assume that we were able to open file
		// ready to write to the file
		write(fd, result, strlen(result)); // write to file
		close(fd);
		
	}
	
	else
	{
		// output data to stdout
		write(STDOUT_FILENO, result, strlen(result)); // write to stdout
		
	}
	
	// free the calloc'ed memory used in the program
	free(buf);
	free(result);	
	return 0;
}
