C = gcc
FLAGS = -Wall -o
NLINK = -c



all : mpg.o common.o
	$(C) $(FLAGS) mpg $^

common.o : common.c common.h
	$(C) $(NLINK) $^
