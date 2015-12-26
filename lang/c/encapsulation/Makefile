CC=gcc

CFLAGS=-g -Wall -Werror -Wextra # -Wpedantic
ifeq ($(CC), clang)
CFLAGS+=-Weverything
endif

queue: main.o queue.o
	$(CC) $(CFLAGS) main.o queue.o -o queue

main.o: main.c queue.h
	$(CC) $(CFLAGS) -c main.c -o main.o

queue.o: queue.c queue.h
	$(CC) $(CFLAGS) -c queue.c -o queue.o

clean:
	rm -rf *.o queue core core.*

run: queue
	./queue
