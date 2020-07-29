CFLAGS=-g -Wall -Wextra -O3 -fopenmp
LDFLAGS=-lm
a.out: digits4.c
	gcc $(CFLAGS) $^ $(LDFLAGS)

run: a.out
	OMP_NUM_THREADS=8 time ./a.out

clean:
	rm -rf a.out core *.log vgcore*

chk: a.out
	valgrind --leak-check=full ./a.out
