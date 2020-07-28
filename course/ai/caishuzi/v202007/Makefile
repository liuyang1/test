CFLAGS=-g -Wall -O3
LDFLAGS=-lm
a.out: digits4.c
	gcc $(CFLAGS) $^ $(LDFLAGS)

run: a.out
	time ./a.out

clean:
	rm -rf a.out core *.log

chk: a.out
	valgrind --leak-check=full ./a.out
