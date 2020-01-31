#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "common.h"

// constants
#define DEFAULT 0
#define FILESRC 1
#define PIPE_RD fd1[0]
#define PIPE_WR fd1[1]

int main(int argc, char *argv[])
{
	// Initialize the flags
	int in_src = DEFAULT; // input source flag
	int out_src = DEFAULT; // output source flag
	int blocks = 10; // number of characters in a block
	char* block_str = "10"; // block as a string
	char* mask = (char*) calloc(15, sizeof(char)); // allocate memory for mask string
	strcpy(mask, "\0"); // initialize mask with end character
	
	// Values needed in the flag-updating section of the program
	int argvpos = 0; // start at argv[0]
	char* errmsg = "ERROR tx: invalid input entered\n";
	int argcpos = 1; 
	char* i_filename = "0"; // storage place for the name of the input file
	char* o_filename = "0"; // storage place for the name of the output file
	
	int b_updated = 0; // used to check if blocks flag was already updated
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
						err("ERROR tx: no input for argument\n");
					}
					i_filename = argv[++argvpos];
					//printf("input source flag updated\n"); // test
					
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
						err("ERROR tx: no input for argument\n");
					}
					o_filename = argv[++argvpos];
					//printf("output source flag updated\n"); // test
					
					break;
					
				case 'b': // the 'b' character was found
					// check if blocks flag has already been updated
					if(b_updated > 0)
					{
						++argcpos; // to keep the string count on track
						break; // if so, don't update again
					}
					
					// update the blocks flag
					++argcpos;
					if(argcpos >= argc)
					{
						err("ERROR tx: no input for argument\n");
					}
					blocks = atoi(argv[++argvpos]); // store <num> int in blocks flag
					block_str = strdup(argv[argvpos]); // store the <num> as a string
					
					
					// exit the program with an error message if not a valid number
					if(blocks == 0)
					{
					err(errmsg);
					}
					
					++b_updated; // set the flag to not update this variable again
					//printf("blocks flag updated\n"); // test
					
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
					//printf("the mask is: %s\n", mask); // test
					//printf("string length is: %d\n", strlen(mask)); // test
					
					break;
					
			}
		}
		
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
			//printf("the mask is: %s\n", mask); // test
			//printf("string length is: %d\n", strlen(mask)); // test
			
		}
		++argcpos;
		
	} // end while
	
	// check to see if the mask was updated, error if not updated
	if(m_updated == 0)
	{
		err("ERROR tx: mask must be provided\n");
	}
	
////////////////////////////////////////////////////////////////////////	
////////// Create the fork to open the twist and xor programs //////////
////////////////////////////////////////////////////////////////////////

	pid_t pid; // process ID
	
	//printf("Parent program (pid:%d)\n", (int) getpid()); // test
	
	int fd1[2];
	
	// set up the pipe
	if(pipe(fd1) < 0)
	{
		err("ERROR tx: pipe failed\n");
	}	

	// fork the process
	if((pid = fork()) < 0)
	{
		// the fork has failed
		err("ERROR tx: fork failed\n");
	}
	
	else if(pid > 0)
	{
		// parent follows this path, this could be the twist
		close(PIPE_RD);						// close the parent read
		dup2(PIPE_WR, STDOUT_FILENO);			
		
			// user entered an input filename
			char* myargs[6];
			myargs[0] = strdup("./twist");	// program: twist
			myargs[1] = strdup("-b"); 		// argument: block size argument
			myargs[2] = strdup(block_str); // argument: block size
			myargs[3] = strdup("-i");		// argument: input filename argument
			myargs[4] = strdup(i_filename);	// argument: input filename
			myargs[5] = NULL; 				// end of the array
			
		// if user did not enter in an input filename
		if(in_src == DEFAULT)
		{
			// update myargs to exclude input file flag
			myargs[3] = NULL;
			myargs[4] = NULL;
		}
				
		// open the twist program
		
		int code = execvp(myargs[0], myargs); // run the twist program
		if(code < 0) 
		{
			err("ERROR tx: unable to execute program\n");
		}
		
		//printf("This is the parent program\n"); // test
	
	}
	
	else
	{
		// child will go down this path, xor program will run here
		 
		close(PIPE_WR);					// close the parent write
		dup2(PIPE_RD, STDIN_FILENO);		
		
		// pipes should be set up and ready at this point
		
		// pass arguments to the program that were defined in beginning
		
		char* myargs[5]; 					// to input to the xor program
		myargs[0] = strdup("./xor");		// program: xor
		myargs[1] = strdup(mask);			// argument: mask
		myargs[2] = strdup("-o");			// argument: output filename argument
		myargs[3] = strdup(o_filename);		// argument: output filename
		myargs[4] = NULL;
		
		// if user did not enter in an output filename
		if(out_src == DEFAULT)
		{
			// update myargs to exclude input file flag
			myargs[2] = NULL;
			myargs[3] = NULL;
		}
		
		// now should open the xor program
		int code = execvp(myargs[0], myargs); // run the xor program
		
		if(code < 0) 
		{
			err("ERROR tx: unable to execute program\n");
		}
		
		//printf("This is the child program\n"); // test
	}
	
	free(mask); // free the calloc'ed space
//*/
	return 0;
	
}
