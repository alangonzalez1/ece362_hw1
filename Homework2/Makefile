C = gcc
FLAGS = -Wall -o
NLINK = -c

all : tx twist

tx : tx.o common.o
	$(C) $(FLAGS) tx $^

twist : twist.o common.o
	$(C) $(FLAGS) twist $^

common.o : common.c common.h
	$(C) $(NLINK) $^

