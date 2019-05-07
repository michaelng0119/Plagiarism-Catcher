#target: dependencies
#<tab> rule

plagiarismCatcher: driver.o
	g++ -o plagiarismCatcher driver.o

driver.o: driver.cpp
	g++ -c driver.cpp
