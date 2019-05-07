Michael Nguyen (myn99)
Spring 2019

The program, plagiarismCatcher, finds similarities between files in a directory and displays them in an orderly fashion:
# of collisions: File 1 name, File 2 name

In order to run the program, place the directory of text files into the same directory as the makefile & driver.
Type "make" into the command line and run by using the following input

./plagiarismCatcher string_a int_b int_c

string_a = name of directory, no slashes
int_b = size of words in a chunk (6 is a good number)
int_c = threshold for collisions (aim for above 100)
