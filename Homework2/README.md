ECE 362
Homework 2
Alan Gonzalez - Victor Albarran

Three programs are included in this compressed folder.

		twist.c

The twist program reverses the order of bytes read in and outputs 
the bytes in reverse order. The bytes will be processed in blocks 
(less than 1K). This program can read up to 1K characters. The program
will receive input from the terminal unless specified by an argument and
a filename.

-i <filename>	input filename (default: stdin)

-o <filename>	output filename (default: stdout)

-b <num>	number of characters in a block to reverse (default: 10)


		xor.c

The xor program will process a string and XOR a block of characters with a
provided mask (less than 10 characters in length). The program will receive
input from the terminal unless specified by an argument and a filename. The
program will output to the terminal unless specified by an argument and a 
filename.

-i <filename>	input filename (default: stdin)

-o <filename>	output filename (default: stdout)

<string>	the mask which must be provided and be less than 10 characters


		tx.c

This program will utilize a unix pipe to fork two processes (twist and xor)
and send the output of twist through the pipe to the input of xor.

-i <filename>	input filename (default: stdin)

-o <filename>	output filename (default: stdout)

-b <num>	number of characters in a block to reverse (default: 10)

<string>	the mask which must be provided and be less than 10 characters