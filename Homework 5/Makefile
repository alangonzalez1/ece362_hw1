C = gcc
FLAGS = -Wall -o
NLINK = -c
PTHREAD = -pthread

four_threads : four_threads.o common.o
	$(C) $(FLAGS) test $^ $(PTHREAD)

common.o : common.c common.h
	$(C) $(NLINK) $^
