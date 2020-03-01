#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "common.h"

pthread_t ntid;

void printids(const char* s)
{
	pid_t		pid;		// for the process ID
	pthread_t	tid;		// for the thread ID
	
	pid = getpid();			// get the pid of the thread
	tid = pthread_self();	// get the thread ID
	printf("%s pid %lu tid %lu (0x%lx)\n", s, (unsigned long) pid, 
			(unsigned long) tid, (unsigned long)tid);
}

void* thr_fn(void *arg)
{
	printids("new thread: ");
	return((void*)0);
}

int main(void)
{
	int status;
	int i = 0;
	while(i <= 4)
	{
		status = pthread_create(&ntid, NULL, thr_fn, NULL);
		if(status !=0)
		{
			err("unable to create thread");
		}
		++i;
	}
	printids("main thread: ");
	exit(0);
}
