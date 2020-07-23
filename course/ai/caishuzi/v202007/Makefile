CFLAGS=-g -Wall
LDFLAGS=-lm
a.out: digits4.c
	gcc $(CFLAGS) $(LDFLAGS) $^

run: a.out
	./a.out

clean:
	rm -rf a.out

chk: a.out
	valgrind --leak-check=full ./a.out
