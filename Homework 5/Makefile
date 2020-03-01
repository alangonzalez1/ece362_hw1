C = gcc
FLAGS = -Wall -o
NLINK = -c

four_threads : four_threads.o common.o
	$(C) $(FLAGS) test $^

common.o : common.c common.h
	$(C) $(NLINK) $^
