C = gcc
FLAGS = -Wall -o
NLINK = -c

all : tx twist xor

tx : tx.o common.o
	$(C) $(FLAGS) tx $^

twist : twist.o common.o
	$(C) $(FLAGS) twist $^

xor : xor.o common.o
	$(C) $(FLAGS) xor $^

common.o : common.c common.h
	$(C) $(NLINK) $^

