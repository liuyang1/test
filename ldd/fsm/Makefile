CFLAGS=-O2 -Wall
TARGET=ngrep
CC=g++
TARGET=ngrep

all:main.o fsm.o
	$(CC) $(CFLAGS) main.o fsm.o -o $(TARGET)
main.o:main.c fsm.h
	$(CC) $(CFLAGS) -c main.c -o main.o
fsm.o:fsm.cpp fsm.h
	$(CC) $(CFLAGS) -c fsm.cpp -o fsm.o

clean:
	rm -rf *.o *.out $(TARGET)
test:
	./ngrep target
