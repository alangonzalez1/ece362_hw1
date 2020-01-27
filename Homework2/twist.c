/*
 * twist.c - Twist the input and output it to a desired location
 * 
 * Author: Alan Gonzalez
 * ECE 362
 * 01/19/20
 * 
 * This is the source code file a program that will take in a
 * string of data from a desired source, twist it, and output it to a 
 * desired location
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

// constants
#define DEFAULT 0
#define FILESRC 1

int main(int argc, char *argv[])
{
	// Initialize the flags
	int in_src = DEFAULT; // input source flag
	int out_src = DEFAULT; // output source flag
	int blocks = 10; // number of characters in a block
	
	// Values needed in the flag-updating section of the program
	int argvpos = 0; // start at argv[0]
	char* errmsg = "ERROR twist: invalid input entered\n";
	int argcpos = 1; 
	char* i_filename = "0"; // storage place for the name of the input file
	char* o_filename = "0"; // storage place for the name of the output file
	
////////////////////////////////////////////////////////////////////////	
//////////// update flags based on user-inputted arguments /////////////
////////////////////////////////////////////////////////////////////////

	while(argcpos < argc)
	{
		int char_pos = 0; // position of the character in the string
		
		// look for the "-" character
		char* temp = argv[++argvpos]; // start at argv[1]
		
		// go to the first character
		if(get_char_from_str(temp, char_pos) == '-')
		{
			// check next character 
			switch(get_char_from_str(temp, char_pos+1))
			{
				case 'i': // the 'i' character was found
					in_src = FILESRC; // update the input source flag
					
					// store the input filename string
					++argcpos;
					if(argcpos >= argc)
					{
						err("ERROR twist: no input for argument\n");
					}
					i_filename = argv[++argvpos];
					
					break;
					
				case 'o': // the 'o' character was found
					// store the output filename string
					out_src = FILESRC; // update the output source flag
					
					// store the output filename string
					++argcpos;
					if(argcpos >= argc)
					{
						err("ERROR twist: no input for argument\n");
					}
					o_filename = argv[++argvpos];
					
					break;
					
				case 'b': // the 'b' character was found
					// update the blocks flag
					++argcpos;
					if(argcpos >= argc)
					{
						err("ERROR twist: no input for argument\n");
					}
					blocks = atoi(argv[++argvpos]); // store <num> int in blocks flag
					
					
					// exit the program with an error message if not a valid number
					if(blocks == 0)
					{
					err(errmsg);
					}
					
					break;
				
				default: // an invalid character or no character detected
					err(errmsg);
					break;
					
			}
		}
		
		// the '-' character was not found	
		else
		{
			err("ERROR twist: arguments are required to start with a '-' character\n");
		}
		
		++argcpos;	
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
		int size = read(fd, buf, 1024); // read from filename, store in buf
		buf[strlen(buf) - 1] = '\0'; // trim the string, store in buf
		close(fd); // close the file
		
	}
	
	else
	{
		// load in data from stdin
		int size = read(STDIN_FILENO, buf, 1024); // read from STDIN
		buf[strlen(buf) - 1] = '\0'; // trim the string, store in buf
		
	}
	
////////////////////////////////////////////////////////////////////////
////////////////////////// Modify the input ////////////////////////////
////////////////////////////////////////////////////////////////////////
	
	// error if number of blocks exceeds string length
	if(blocks > strlen(buf))
	{
		err("ERROR twist: number of blocks cannot exceed number of characters in string\n");
	}
	
	// initialize the needed variables to modify the input
	char* group = (char*) calloc(blocks, sizeof(char)); // for storing the block of characters taken from buf
	char* mod_group = (char*) calloc(blocks, sizeof(char)); // for storing the twisted block of characters
	int i; // used in the for loop
	char* result = (char*) calloc(1024, sizeof(char)); // to store the result
	int bk = blocks; // copy blocks variable to change it
	int counter = 0; // to keep track of how many characters read
	char ch; // character that we will be working with
	
	// main algorithm to modify the input
	while(counter < strlen(buf))
	{
			
		for(i=bk-blocks; i<bk; ++i)
		{
			// Parse the string to extract characters into blocks
			if(counter	 >= strlen(buf))
			{
				break;
			}
			ch = get_char_from_str(buf, i); // get character in the ith position
			strncat(group, &ch, 1); // add to group string
			++counter; // update the counter when another character grabbed
		}
		
		// twist the block of characters
		for(i=(strlen(group)-1); i>=0; --i)
		{
			ch = get_char_from_str(group, i); // get character in the ith position
			strncat(mod_group, &ch, 1); // add to modified group string
		}
		
		// ready to store the twisted block in the final result
		strncat(result, mod_group, strlen(mod_group));
		strncpy(group, "", 1); // reset the group string to take in next block
		strncpy(mod_group, "", 1); // reset the string to take in next block
		bk += blocks; // increment bk variable
		
		
	}
	//printf("result is: %s\n", result); // test
		
////////////////////////////////////////////////////////////////////////
////////////////// Output result to desired location ///////////////////
////////////////////////////////////////////////////////////////////////

	// check output source flag
	if(out_src == FILESRC)
	{
		// output data to specified file
		int fd = open(o_filename, O_CREAT | O_WRONLY | O_APPEND, S_IRWXU); 
		// Open a file, write only, create it if needed, truncate if already there
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
		//printf("finished writing\n"); // test
		close(fd);
		
	}
	
	else
	{
		// output data to stdout
		write(STDOUT_FILENO, result, strlen(result)); // write to stdout
		
	}
	
	
	return 0;
}
