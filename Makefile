CC=g++ -Wall

all: main

main: helper.o main.o
	$(CC) -pthread -o main helper.o main.o
	
main.o: helper.cc main.cc
	$(CC) -c helper.cc main.cc

tidy:
	rm -f *.o core

clean:
	rm -f main producer consumer *.o core

#######################################################

#sudoku: main.o sudoku.o
	#g++ main.o sudoku.o -o sudoku

#main.o: main.cpp sudoku.h
	#g++ -Wall -g -c main.cpp -o main.o

#sudoku.o: sudoku.cpp sudoku.h
	#g++ -Wall -g -c sudoku.cpp -o sudoku.o

#clear:
	#rm -f *.o sudoku

#.PHONY: clear
