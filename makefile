CC = g++ #compiler we're using is g++
CFLAGS = -g -Wall #-g = debugging info, -Wall = all warnings

checkers: main.o checker.o node.o
	$(CC) $(CFLAGS) main.o checker.o node.o -o checkers
#main non main cpp and header we're using
checker.o: checker.h checker.cpp
	$(CC) $(CFLAGS) -c checker.cpp
#node not being used yet but it's inported into main
node.o: node.h node.cpp
	$(CC) $(CFLAGS) -c node.cpp

main.o: main.cpp checker.cpp node.cpp
	$(CC) $(CFLAGS) -c main.cpp

run:	checkers
	./checkers

clean: #remove everything
	rm *.o
	rm checkers
