CC=g++ -Wall

all: main

main: helper.o main.o consumer.o producer.o
	$(CC) -pthread -o main helper.o main.o consumer.o producer.o

main.o: helper.cc main.cc consumer.h producer.h
	$(CC) -c helper.cc main.cc

consumer.o: consumer.cc consumer.h helper.h
	$(CC) -c consumer.cc -o consumer.o

producer.o: producer.cc producer.h helper.h
	$(CC) -c producer.cc -o producer.o

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
