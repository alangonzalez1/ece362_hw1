C = gcc
FLAGS = -Wall -o
NLINK = -c

main : mpg.o common.o
	$(C) $(FLAGS) main $^

common.o : common.c common.h
	$(C) $(NLINK) $^
