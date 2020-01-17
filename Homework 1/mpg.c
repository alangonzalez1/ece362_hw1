/*
 * mpg.c - MultiQueue Generator program 
 * 
 * Author: Alan Gonzalez
 * ECE 362
 * Jan 12th, 2020
 * 
 * This program will generate a set of integer arrays that are randomly 
 * populated using command line arguments.
 * 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "common.h"


// constants
#define MAX_Q 20
#define MAX_REQ 100

int main(int argc, char *argv[])
{
	// Initialize the flags
	int nqueues = 1;
	int nrequests = 5;
	int min = 0;
	int max = 20;
	
	// Initialize values needed for the flag-updating section of the program
	int argvpos = 0;
	char* errmsg = "ERROR mpg: invalid input entered\n";
	int argcpos = 1;
	
	// Initialize values needed for the queue-generating section of the program
	int x; // for loop
	int RforQ; // spaces available for one queue
	int m; // randomly generated number of spaces for the queue
	
////////////////////////////////////////////////////////////////////////	
//////////// update flags based on user-inputted arguments /////////////
////////////////////////////////////////////////////////////////////////

	while(argcpos < argc)
	{
		int pos = 0;
		// Look for the "-" character
		char* temp = argv[++argvpos];
		//printf("argv after looking for the '-' character is: %s\n", argv[argvpos]); // test

	if (get_char_from_str(temp, pos) == '-')
	{
		//printf("The '-' character was found.\n"); // test
		switch (get_char_from_str(temp, ++pos)) // check next character
		{
			case 'q': // if 'q' character found then
				nqueues = atoi(argv[++argvpos]); // store <num> int in nqueues
				++argcpos;
				if(nqueues == 0)
				{
					err(errmsg);
				}
				//printf("the number in nqueues is: %d\n", nqueues); // test
				break;

			case 'r': // if 'r' character found then
				nrequests = atoi(argv[++argvpos]); // store <num> int in nrequests
				++argcpos;
				if(nrequests == 0)
				{
					err(errmsg);
				}
				//printf("the number in nrequests is: %d\n", nrequests); // test
				break;

			case 'm': // if 'm' character found then
				switch (get_char_from_str(temp, ++pos)) // check next character
				{
					case 'a':
						if(get_char_from_str(temp, ++pos) == 'x')
						{
							max = atoi(argv[++argvpos]); // store <num> int in max
							++argcpos;
							if (max == 0)
								err(errmsg);
							//printf("The number in max is: %d\n", max); // test
						}
						else
							err(errmsg);

						goto found;
						break;
					
					case 'i':
						if(get_char_from_str(temp, ++pos) == 'n')
						{
							min = atoi(argv[++argvpos]); // store <num> int in min
							++argcpos;
							if (max == 0)
								err(errmsg);
							//printf("The number in min is %d\n", min); // test
						}
						else
							err(errmsg);
						
						goto found;
						break;

					default:
						err(errmsg);
						break;
				}

			default:
				err(errmsg);
				break;
		}
	}

	// Was unable to find the "-" character
	else
	{
		err("ERROR mpg: arguments are required to start with a '-' character\n");
	}
	
found:	
	//printf("Flag successfully updated\n"); // test
	++argcpos;	
	}

////////////////////////////////////////////////////////////////////////
/////////// At this point, all the flags have been updated /////////////
//////// now the program is ready to generate the needed queues ////////
////////////////////////////////////////////////////////////////////////

	srand(time(0)); // seed for the rand function
	int Q[nqueues][MAX_REQ+2]; // Multidimensional array for storing the queues 
		// and their sums and the number of requests for each
	int counter = 0; // counter for the number of times the loop has been executed
	int Rnum; // where the random number will be generated
	
	// copy the flags in order to modify them
	int nR = nrequests; // copy the requests flag
	int numQ = nqueues; // copy the queues flag
	
	// will loop for all queues
	while(numQ > 0)
	{
		// if only have one queue or the last queue
		if(numQ == 1) // the last queue
		{
			Q[counter][0] = 0; // initialize the sum to 0
			Q[counter][1] = nR; // store the number of requests for the queue
			
			for(x=0; x<nR; ++x)
			{
				Rnum = randn(min, max); // generate random number between min and max
				//printf("Random number is: %d\n", Rnum); // test
				Q[counter][x+2] = Rnum; // store the random number in the queue
				Q[counter][0] +=Rnum; // add the random number to the total sum
				//printf("%d\n", Q[counter][x+2]); // test
			}
			
			//printf("The sum is: %d\n", Q[counter][0]); // test
			--numQ; // decrement the number of needed queues after generating one
			++counter; // update the counter
			// numQ should be 0 at this point
		}
		
		else
		{
			Q[counter][0] = 0; // initialize the sum to 0

			
			RforQ = nR - (numQ - 1); // to ensure all queues get at least 1 num
			m = randn(1, RforQ); // number of requests that the first queue will receive
			
			Q[counter][1] = m; // store the number of requests for the queue
			
			for(x=0; x<m; ++x)
			{
				Rnum = randn(min, max); // generate random number between min and max
				//printf("Random number is: %d\n", Rnum); // test
				Q[counter][x+2] = Rnum; // store the random number in the queue
				Q[counter][0] += Rnum; // add the random number to the total sum
				//printf("%d\n", Q[counter][x+2]); // test
			}
			
			//printf("The sum is: %d\n", Q[counter][0]); // test
			nR -= m; // update the number of requests because we used some of them
			--numQ;  // decrement the number of needed queues after generating one
			++counter; // update the counter
		}
		//printf("\n"); // test
		
	}
	
////////////////////////////////////////////////////////////////////////
///////////// Traverse the sum column and find lowest sum //////////////
////////////////////////////////////////////////////////////////////////

	// find the position of the lowest sum
	int pos = 0; // position of the lowest sum
	int minSum = Q[0][0]; // set min to be the first entry in the array
	int value; // store the value that we are working with
	
	for(x=0; x<nqueues; ++x)
	{
		value = Q[x][0];
		printf("Queue %d sum: %d\n", x+1, value); // test
		if(minSum > value)
		{
			minSum = value; // update the lowest sum value
			pos = x; // update the position of the lowest sum queue
			//printf("position is now: %d\n", pos); // test
		}
	}
	
	putchar('\n');	
	// the position of the lowest sum has been found
	// now to print the corresponding queue
	
	printf("Values in lowest-sum queue\n");
	for(x=0; x<Q[pos][1]; ++x)
	{
		value = Q[pos][x+2];
		if(x==0)
		{
			printf("%d", value);
		}
		else 
		{
			printf(", %d", value);
		}
	}
	putchar('\n');
	return 0;
	
}
