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

	// Look for the "-" character
	char* temp = argv[1];
	if (get_char_from_str(temp, 0) == '-')
	{
		printf("The '-' character was found.\n");
	}

	// Was unable to find the "-" character
	else
	{
		fprintf(stderr, "mpg: invalid input entered\n");
		exit(1);
	}
	return 0;
}
