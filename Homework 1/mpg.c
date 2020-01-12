#include <stdio.h>
#include <stdlib.h>
#include "common.h"

int main(int argc, char *argv[])
{
	// Initialize the flags
	int nqueues = 1;
	int nrequests = 5;
	int min = 0;
	int max = 20;

	// initialize values needed for the flag-updating section of the code
	int argvpos = 0;
	char* errmsg = "ERROR mpg: invalid input entered\n";
	int argcpos = 1;
	
	//printf("argc starts out as: %d\n", argc); // test
	
	// update flags based on user-inputted arguments
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
				//printf("the number of requests is: %d\n", nrequests); // test
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
	
	
	return 0;
}
